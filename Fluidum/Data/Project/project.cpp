#include "project.h"
#include "../Coding/tab.h"

namespace FD::Internal::Project {

	//���݂̃v���W�F�N�g�̃f�[�^
	struct Data final {
	public:
		std::string projectName{};//project�̖��O
		std::string projectFolderPath{};//project�̂���t�@�C��

		std::string mainCodeFilePath{};//�J�n����\�[�X�t�@�C��
		std::vector<std::string> includeCodeFilePathes{};//�v���W�F�N�g�Ɋ܂߂�t�@�C���̃p�X

	public://�ۑ����Ȃ����
		bool isDataChanged = false;
		bool isDefaultProject = true;

	public:
		void reset() {
			*this = Data();
		}

	}GCurrentData;

	std::mutex GMtx{};


	constexpr std::size_t FluidumProjectFileIdentifier =
		0b01110100'11011001'01100111'01011000'01010000'10110101'10011000'00111000;

}

namespace FD::Internal::Project {

	std::string getCurrentTime() {
		auto time1 = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time();
		auto time2 = std::chrono::floor<std::chrono::days>(time1);
		std::chrono::year_month_day ymd(time2);

		std::chrono::hh_mm_ss time(std::chrono::floor< std::chrono::milliseconds>(time1 - time2));

		std::stringstream ss;
		ss << ymd.year() << '_' << ymd.month() << '_' << ymd.day() << '_'
			<< time.hours().count() << '_' << time.minutes().count() << '_' << time.seconds().count() << '_' << time.subseconds().count();
		return ss.str();
	}

	//\��/�ɂ�����
	//���K��
	std::string correctFilePath(const char* path) {
		std::string result = std::filesystem::path(path).lexically_normal().generic_string();
		auto itr = result.begin();
#ifdef BOOST_OS_WINDOWS
		while (true) {
			itr = std::find(itr, result.end(), '\\');

			if (itr != result.end())
				*itr = '/';
			else
				break;
		}
#endif
		return result;
	}

}

FD::ProjectWrite::ProjectWrite(Internal::PassKey) {
	using namespace Internal::Project;

	//���̃v���W�F�N�g�͏����D
	//�t�H���_���ƑS�ď����č�蒼��
	std::filesystem::remove_all(Internal::Resource::TempProjectFolderPath);
	if (!std::filesystem::create_directory(Internal::Resource::TempProjectFolderPath))
		throw std::runtime_error("Failed to create TempProject directory.");

	//�f�t�H���g�ŉ��̃v���W�F�N�g���쐬
	std::string time = Internal::Project::getCurrentTime();

	//���̃v���W�F�N�g�l�[���͎��Ԃɂ���
	const std::string projectName = time;

	std::string path = Internal::Resource::TempProjectFolderPath;
	GCurrentData.projectFolderPath = path;

	path += time += ".fproj";

	std::ofstream ofs(path, std::ios::out);
	if (!ofs)
		throw std::runtime_error("Failed to write backup file.");

	GCurrentData.projectName = projectName;

	//backup code�t�H���_�̍쐬
	this->makeBackupFolder();
	this->makeCodeFolder();

	this->write(path.c_str());
}

//Utils����
bool checkExtension(const std::string& path, const std::string& extension) {
	if (path.size() < extension.size())
		return false;

	std::string compare;
	compare.resize(extension.size());

	std::copy(path.end() - extension.size(), path.end(), compare.begin());

	return compare == extension;
}

void FD::ProjectWrite::createNewProject(const Project::CreateProjectInfo& info) const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	using namespace Internal::Project;

	//��O���ł��ꍇ�ɂ��Ƃ̏��ɖ߂�
	Data temp{};
	try {
		temp = std::move(GCurrentData);
	}
	catch (...) {
		abort();
	}

	try {
		//�V��������ւ���
		GCurrentData.reset();

		std::string projectFolderPath = correctFilePath(info.projectFolderPath);
		GCurrentData.projectFolderPath = std::move(projectFolderPath);

		GCurrentData.projectName = info.projectName;

		//�m�F
		if (GCurrentData.projectFolderPath.back() != '/')
			GCurrentData.projectFolderPath.push_back('/');
		GCurrentData.projectFolderPath += (GCurrentData.projectName + '/');

		//�f�t�H���g�ł͂Ȃ�
		GCurrentData.isDefaultProject = false;

		//ProjectFolder���쐬
		//���ɑ��݂���
		if (std::filesystem::is_directory(GCurrentData.projectFolderPath))
			throw Project::ExceptionType::AlreadyProjectFolderExist;
	}
	catch (const std::exception&) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}

	//directory�쐬
	try {
		if (!std::filesystem::create_directory(GCurrentData.projectFolderPath))
			throw std::runtime_error("Failed to create ProjectFolder directory.");
	}
	catch (const std::exception&) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}

	try {
		//Backup Code Folder���쐬
		this->makeBackupFolder();
		this->makeCodeFolder();

		//.fproj���쐬
		std::string path = GCurrentData.projectFolderPath;
		path += (GCurrentData.projectName + ".fproj");
		this->write(path.c_str());
		this->updateHistory();
	}
	//��O���ł�����Ƃɖ߂�
	catch (const std::exception&) {
		GCurrentData = std::move(temp);

		//project directory�폜
		std::filesystem::remove_all(info.projectFolderPath);

		std::rethrow_exception(std::current_exception());
	}

}

void FD::ProjectWrite::loadExistProject(const char* path) const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	using namespace Internal::Project;

	//��O���ł��ꍇ�ɂ��Ƃ̏��ɖ߂�
	Data temp{};
	try {
		temp = GCurrentData;
	}
	catch (...) {
		abort();
	}

	//�V��������ւ���
	GCurrentData.reset();

	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs) {
		GCurrentData = std::move(temp);
		throw Project::ExceptionType::FailedToOpenProjectFile;
	}

	std::string data;

	std::size_t identifier = 0;

	try {
		ifs.read(reinterpret_cast<char*>(&identifier), 8);//���ʂ��鐔�l

		if (identifier != FluidumProjectFileIdentifier)
			throw Project::ExceptionType::IllegalFile;

		std::getline(ifs, data);
		std::getline(ifs, data);//Time
		std::getline(ifs, data);
		std::getline(ifs, data);//ProjectName

		std::getline(ifs, data);
		GCurrentData.projectName = data;

		std::getline(ifs, data);
		std::getline(ifs, data);
		GCurrentData.projectFolderPath = data;

		std::getline(ifs, data);
		std::getline(ifs, data);
		GCurrentData.mainCodeFilePath = data;

		std::getline(ifs, data);
		std::size_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == "Next")
				break;
			GCurrentData.includeCodeFilePathes.emplace_back(data);

			counter++;
			if (counter > 10000)
				throw Project::ExceptionType::BrokenFile;
		}

		this->readTabInfo(ifs);

		this->updateHistory();
	}
	catch (const std::exception&) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}
	catch (const Project::ExceptionType) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}


	GCurrentData.isDataChanged = false;

	//�f�t�H���g�ł͂Ȃ�
	GCurrentData.isDefaultProject = false;




}

void FD::ProjectWrite::saveAs(const char* newName, const char* dstProjectFolderPath) const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	std::string projectFolderPath = Internal::Project::correctFilePath(dstProjectFolderPath);

	//�܂��C�R�s�[��ɓ������O�̃t�H���_�����邩�`�F�b�N
	if (!std::filesystem::is_directory(projectFolderPath))
		throw Project::ExceptionType::AlreadyProjectFolderExist;

	//���݂̃t�H���_���ړ���폜����Ă��Ȃ���
	this->checkIsProjectFolderExist();

	//copy����
	std::filesystem::copy(Internal::Project::GCurrentData.projectFolderPath, projectFolderPath, std::filesystem::copy_options::recursive);

	//copy�����猻�݂̃v���W�F�N�g�����ւ���
	using namespace Internal::Project;
	GCurrentData.projectName = newName;

	GCurrentData.projectFolderPath = projectFolderPath;
	if (GCurrentData.projectFolderPath.back() != '/')
		GCurrentData.projectFolderPath.push_back('/');


	this->updateHistory();

	GCurrentData.isDataChanged = false;
	GCurrentData.isDefaultProject = false;
}

void FD::ProjectWrite::save() const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	assert(GCurrentData.projectFolderPath.back() == '/');

	//�v���W�F�N�g�t�H���_�����邩�m�F
	this->checkIsProjectFolderExist();

	//�ۑ�
	const std::string path = GCurrentData.projectFolderPath + GCurrentData.projectName + ".fproj";
	this->write(path.c_str());

	GCurrentData.isDataChanged = false;
}

void FD::ProjectWrite::backup() const {
	using namespace Internal::Project;
	assert(GCurrentData.projectFolderPath.back() == '/');

	std::string path = Internal::Project::GCurrentData.projectFolderPath + "Backup/";
	path += Internal::Project::getCurrentTime() += ".backup";

	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	//backup�t�H���_��������Ă�������
	this->makeBackupFolder();

	this->write(path.c_str());
}

void FD::ProjectWrite::setMainCodePath(const char* path) const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	GCurrentData.mainCodeFilePath = path;
}

void FD::ProjectWrite::addIncludeCodePath(const char* path) const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	GCurrentData.includeCodeFilePathes.emplace_back(path);
}

void FD::ProjectWrite::write(const char* path) const {
	std::ofstream ofs(path, std::ios::out | std::ios::binary);

	if (!ofs)
		throw std::runtime_error("Failed to write project file.");

	using namespace Internal::Project;

	//ull��64bit�ȏオ�񑩂���Ă���̂�8byte�Œ�ŏ�������
	ofs.write(reinterpret_cast<const char*>(&FluidumProjectFileIdentifier), 8);

	ofs << "Fluidum: " << path << std::endl;

	ofs << "Time" << std::endl;
	ofs << getCurrentTime() << std::endl;

	ofs << "ProjectName" << std::endl;
	ofs << GCurrentData.projectName << std::endl;

	ofs << "ProjectFilePath" << std::endl;
	ofs << GCurrentData.projectFolderPath << std::endl;

	ofs << "MainSourceCodeFilePath" << std::endl;
	ofs << GCurrentData.mainCodeFilePath << std::endl;

	ofs << "IncludeFilePathes" << std::endl;
	for (const auto& x : GCurrentData.includeCodeFilePathes) {
		ofs << x << std::endl;
	}
	ofs << "Next" << std::endl;

	ofs << "CodingTab" << std::endl;
	{
		std::lock_guard<std::mutex> lock(Internal::Coding::TabData::mtx);
		for (const auto& x : Internal::Coding::TabData::filePathes) {
			ofs << x << std::endl;
		}
		ofs << "Next" << std::endl;
	}

	ofs << "End" << std::endl;

}

void FD::ProjectWrite::makeBackupFolder() const {
	using namespace Internal::Project;

	std::string backupFolderPath = GCurrentData.projectFolderPath;
	backupFolderPath += "Backup/";

	//Backup�f�B���N�g�����Ȃ���΍��
	//����Ȃ炻�̂܂�
	if (!std::filesystem::is_directory(backupFolderPath)) {
		if (!std::filesystem::create_directory(backupFolderPath))
			throw std::runtime_error("Failed to create Backup directory.");
	}
}

void FD::ProjectWrite::makeCodeFolder() const {
	using namespace Internal::Project;

	std::string codeFolderPath = GCurrentData.projectFolderPath;
	codeFolderPath += "Code/";

	//Code�f�B���N�g�����Ȃ���΍��
	//����Ȃ炻�̂܂�
	if (!std::filesystem::is_directory(codeFolderPath)) {
		if (!std::filesystem::create_directory(codeFolderPath))
			throw std::runtime_error("Failed to create Code directory.");
	}
}

void FD::ProjectWrite::checkIsProjectFolderExist() const {
	using namespace Internal::Project;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath))
		throw Project::ExceptionType::NotFoundProjectFolder;

}

namespace FD::Internal::Project {
	std::string getYMDH() {
		auto time1 = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time();
		auto time2 = std::chrono::floor<std::chrono::days>(time1);
		std::chrono::year_month_day ymd(time2);
		std::chrono::hh_mm_ss time(std::chrono::floor< std::chrono::milliseconds>(time1 - time2));

		std::stringstream ss;
		ss << ymd.year() << '_' << ymd.month() << '_' << ymd.day() << '_'
			<< time.hours().count();
		return ss.str();
	}
}

void FD::ProjectWrite::updateHistory() const {
	using namespace Internal::Project;

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	std::deque<Project::HistoryInfo> temp(50);

	//��������deque�ɑS�ēǂݍ���

	const std::string currentPath = GCurrentData.projectFolderPath + GCurrentData.projectName + ".fproj";
	uint16_t index = std::numeric_limits<uint16_t>::max();
	bool isNewFile = true;//�V�K�t�@�C���ŗ����ɂ͂Ȃ�

	//100�܂�
	for (uint16_t i = 0; i < 50; i++) {
		//projectname path time�̏�
		std::getline(ifs, temp[i].projectName);
		std::getline(ifs, temp[i].projectFilePath);
		std::getline(ifs, temp[i].ymd_h);

		if (temp[i].projectFilePath == currentPath) {
			index = i;
			isNewFile = false;
		}
	}

	//���݂̃t�@�C��������Έ�ԏ�ɂ����Ă���
	if (!isNewFile) {
		temp.emplace_front(temp.at(index));
		temp.erase(temp.begin() + index + 1);
	}
	//�V�K
	else {
		Project::HistoryInfo info;
		info.projectFilePath = currentPath;
		info.projectName = GCurrentData.projectName;
		info.ymd_h = getYMDH();
		temp.emplace_front(std::move(info));
	}


	//��������
	std::ofstream ofs(Internal::Resource::RecentProjectHistoryFilePath, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to update FluidumProject history.");

	for (const auto& x : temp) {
		ofs << x.projectName << std::endl;
		ofs << x.projectFilePath << std::endl;
		ofs << x.ymd_h << std::endl;
	}

}

void FD::ProjectWrite::readTabInfo(std::ifstream& ifs) const {
	using namespace Internal::Project;

	std::lock_guard<std::mutex> lock(Internal::Coding::TabData::mtx);

	std::string data{};
	std::size_t counter = 0;

	while (true) {
		std::getline(ifs, data);
		if (data == "Next")
			break;
		Internal::Coding::TabData::filePathes.emplace_back(data);

		counter++;
		if (counter > 1000)
			throw Project::ExceptionType::BrokenFile;
	}
}


std::array<FD::Project::HistoryInfo, 50> FD::ProjectRead::getProjectHistory() const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(GMtx);

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	if (!ifs)
		throw std::runtime_error("Failed to load history file.");

	std::array<Project::HistoryInfo, 50> result{};

	//50�܂�
	for (std::size_t i = 0; i < std::tuple_size_v<decltype(result)>; i++) {
		//projectname path time�̏��œ����Ă���
		std::getline(ifs, result[i].projectName);
		std::getline(ifs, result[i].projectFilePath);
		std::getline(ifs, result[i].ymd_h);
	}

	return result;
}

bool FD::ProjectRead::isDataChanged() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.isDataChanged;
}

std::vector<std::string> FD::ProjectRead::getIncludeCodeFilePathes() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.includeCodeFilePathes;
}

std::string FD::ProjectRead::getMainCodeFilePath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.mainCodeFilePath;
}

std::string FD::ProjectRead::getProjectFolderPath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectFolderPath;
}

std::string FD::ProjectRead::getBackupFolderPath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectFolderPath + "Backup/";
}

std::string FD::ProjectRead::getCodeFolderPath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectFolderPath + "Code/";
}

std::string FD::ProjectRead::getProjectName() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectName;
}

bool FD::ProjectRead::isDefaultProject() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.isDefaultProject;
}

FD::Project::CodeType FD::ProjectRead::getCurrentMainCodeType() const {
	using namespace Internal::Project;

	if (GCurrentData.mainCodeFilePath.empty())
		return Project::CodeType::Empty;

	std::string extension{};
	(void)std::find_if(
		GCurrentData.mainCodeFilePath.rbegin(),
		GCurrentData.mainCodeFilePath.rend(),
		[&](char x) {
			extension.insert(extension.begin(), x);
			return x == '.';
		}
	);

	if (extension == ".py")
		return Project::CodeType::Python;
	else if (extension == ".lua")
		return Project::CodeType::Lua;
	else if (extension == ".as")
		return Project::CodeType::AngelScript;

	return Project::CodeType::Error;
}
