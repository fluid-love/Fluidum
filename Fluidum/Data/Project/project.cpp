#include "project.h"
#include <Windows.h>
#undef max //collision std::numerical_limit<T>::max

#include "../Coding/tab.h"
#include "../Scene/scene.h"

namespace FD::Internal::Project {


	struct SourceCodeFiles final {
		std::string mainCodeFilePath{};//entry file
		std::vector<std::string> includeCodeFilePathes{};//プロジェクトに含めるファイルのパス
	}GFiles;

	//現在のプロジェクトのデータ
	struct Data final {
	public:
		std::string projectName{};
		std::string projectFilePath{};
		std::string projectFolderPath{};

	public:
		bool isTemp = true;

	public:
		//vector basic_stringともmove後は有効であることが保証されている
		void reset() {
			*this = Data();
		}

	}GCurrentData;

	std::mutex GMtx{};

	constexpr std::size_t FluidumProjectFileIdentifier =
		0b01110100'11011001'01100111'01011000'01010000'10110101'10011000'00111000;

	using namespace ::FD::Internal::Coding;

	struct BackUpInfo final {
		Data data{};

		//tab
		std::string displayFilePath{};
		std::vector<std::string> filePathes{};
	};
}

void FD::ProjectWrite::save_thread() {
	using namespace Internal::Coding;

	bool request = this->saveThread.get_stop_token().stop_requested();

	while (!request) {
		request = this->saveThread.get_stop_token().stop_requested();
		if (TabData::save) {
			std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
			this->save_tab();
			TabData::save.store(false);
		}
		if (Internal::Scene::Data::save) {
			std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
			this->save_scene();
			Internal::Scene::Data::save.store(false);
		}
	}

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

	//\を/にかえる
	//正規化
	std::string correctFolderPath(const char* path) {
		std::string result = std::filesystem::path(path).lexically_normal().generic_string();
#ifdef BOOST_OS_WINDOWS
		auto itr = result.begin();
		while (true) {
			itr = std::find(itr, result.end(), '\\');

			if (itr != result.end())
				*itr = '/';
			else
				break;
		}
#endif
		if (result.back() != '/')
			result.push_back('/');
		return result;
	}

}

//Constructor
FD::ProjectWrite::ProjectWrite(Internal::PassKey) {
	using namespace Internal::Project;

	//remove last temp project.
	//create new temp project.
	std::filesystem::remove_all(Internal::Resource::TempProjectFolderPath);
	if (!std::filesystem::create_directory(Internal::Resource::TempProjectFolderPath))
		throw std::runtime_error("Failed to create TempProject directory.");

	//temp project name -> current time
	std::string time = Internal::Project::getCurrentTime();
	const std::string projectName = time;

	//path -> Fluidum/Project/TempProject
	std::string path = Internal::Resource::TempProjectFolderPath;
	GCurrentData.projectFolderPath = path;

	path += time += ".fproj";
	GCurrentData.projectFilePath = path;

	std::ofstream ofs(path, std::ios::out);
	if (!ofs)
		throw std::runtime_error("Failed to create project file.");

	GCurrentData.projectName = projectName;

	//create: backup code projectfiles
	this->tryCreateProjectFilesFolder();
	this->tryCreateBackupFolder();
	this->tryCreateSrcFolder();
	this->tryCreateFilesFile();
	this->tryCreateTabFile();
	this->tryCreateSceneFile();
	this->tryCreateFunctionFile();

	this->writeProjectInfo(path.c_str());
}

//Utilsいき
//bool checkExtension(const std::string& path, const std::string& extension) {
//	if (path.size() < extension.size())
//		return false;
//
//	std::string compare;
//	compare.resize(extension.size());
//
//	std::copy(path.end() - extension.size(), path.end(), compare.begin());
//
//	return compare == extension;
//}

void FD::ProjectWrite::createNewProject(const Project::CreateProjectInfo& info) const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	using namespace Internal::Project;

	Data temp{};

	try {
		temp = std::move(GCurrentData);
	}
	catch (...) {
		throw Project::ExceptionType::Unexpected;
	}

	try {
		GCurrentData.reset();

		GCurrentData.projectFolderPath = correctFolderPath(info.projectFolderPath);
		GCurrentData.projectName = info.projectName;

		//check
		if (GCurrentData.projectFolderPath.back() != '/')
			GCurrentData.projectFolderPath.push_back('/');
		GCurrentData.projectFolderPath += (GCurrentData.projectName + '/');

		//create ProjectFolder
		if (std::filesystem::is_directory(GCurrentData.projectFolderPath))
			throw Project::ExceptionType::AlreadyProjectFolderExist;
	}
	catch (...) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}

	//directory作成
	try {
		if (!std::filesystem::create_directory(GCurrentData.projectFolderPath))
			throw std::runtime_error("Failed to create ProjectFolder directory.");
	}
	catch (const std::exception&) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}

	try {
		this->tryCreateSrcFolder();
		this->tryCreateProjectFilesFolder();
		this->tryCreateBackupFolder();
		this->tryCreateFilesFile();
		this->tryCreateTabFile();
		this->tryCreateSceneFile();
		this->tryCreateFunctionFile();

		this->save_files();
		this->save_tab();


		//.fproj
		std::string path = GCurrentData.projectFolderPath;
		path += (GCurrentData.projectName + ".fproj");
		GCurrentData.projectFilePath = path;
		this->writeProjectInfo(path.c_str());
		this->updateHistory();
	}
	catch (...) {
		//remove project directory
		try {
			std::filesystem::remove_all(GCurrentData.projectFolderPath);
		}
		catch (...) {
			std::cerr << "Serious Error" << std::endl;
			abort();
		}
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}
	GCurrentData.isTemp = false;
}

void FD::ProjectWrite::loadProject(const char* path) const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	using namespace Internal::Project;

	//例外がでた場合にもとの情報に戻す
	Data temp{};
	SourceCodeFiles tempF{};

	//tab
	std::vector<std::string> temp_tab_filePathes{};
	std::vector<std::string> temp_tab_displayFiles{};

	//scene
	std::vector<FU::Class::ClassCode::CodeType> temp_scene_codes{};

	try {
		temp = GCurrentData;
		tempF = GFiles;
		temp_tab_displayFiles = TabData::displayFiles;
		temp_tab_filePathes = TabData::filePathes;
		temp_scene_codes = Internal::Scene::Data::codes;
	}
	catch (...) {
		throw Project::ExceptionType::Unexpected;
	}

	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs) {
		GCurrentData = std::move(temp);
		throw Project::ExceptionType::FailedToOpenProjectFile;
	}

	try {
		std::size_t identifier = 0;
		ifs.read(reinterpret_cast<char*>(&identifier), 8);//識別する数値

		if (identifier != FluidumProjectFileIdentifier)
			throw Project::ExceptionType::IllegalFile;

		this->readProjectInfo(ifs);
		this->readProjectFiles();
		this->readTabInfo();
		this->readSceneInfo();

		this->updateHistory();
	}
	catch (...) {
		GCurrentData = std::move(temp);
		GFiles = std::move(tempF);

		TabData::displayFiles = std::move(temp_tab_displayFiles);
		TabData::filePathes = std::move(temp_tab_filePathes);

		Internal::Scene::Data::codes = std::move(temp_scene_codes);

		std::rethrow_exception(std::current_exception());
	}
	GCurrentData.isTemp = false;
}

void FD::ProjectWrite::saveAs(const char* newName, const char* dstProjectFolderPath) const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	std::string projectFolderPath = Internal::Project::correctFolderPath(dstProjectFolderPath);

	//まず，コピー先に同じ名前のフォルダがあるかチェック
	if (!std::filesystem::is_directory(projectFolderPath))
		throw Project::ExceptionType::AlreadyProjectFolderExist;

	//現在のフォルダが移動や削除されていないか
	this->checkIsProjectFolderExist();
	this->checkIsProjectFileExist();

	using namespace Internal::Project;

	if (std::filesystem::is_directory(projectFolderPath + newName))
		throw Project::ExceptionType::AlreadyProjectFolderExist;

	//create project folder
	if (!std::filesystem::create_directory(projectFolderPath + newName))
		throw std::runtime_error("Failed to create project folder");

	//例外がでた場合にもとの情報に戻す
	Data temp{};
	try {
		temp = GCurrentData;
	}
	catch (...) {
		abort();
	}

	//copy
	try {
		std::string srcPath = GCurrentData.projectFolderPath + "ProjectFiles/";

		//set new folder path
		GCurrentData.projectFolderPath = projectFolderPath + newName + '/';

		std::filesystem::copy(srcPath, GCurrentData.projectFolderPath + "ProjectFiles/", std::filesystem::copy_options::recursive);

		GCurrentData.projectName = newName;
		GCurrentData.projectFilePath = GCurrentData.projectFolderPath + newName + ".fproj";

		this->updateHistory();
	}
	catch (...) {
		std::filesystem::remove(GCurrentData.projectFolderPath);
		GCurrentData = temp;
		std::rethrow_exception(std::current_exception());
	}


}

void FD::ProjectWrite::save() const {
	//using namespace Internal::Project;
	//std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	//assert(GCurrentData.projectFolderPath.back() == '/');

	////プロジェクトフォルダがあるか確認
	//this->checkIsProjectFolderExist();

	////保存
	//const std::string path = GCurrentData.projectFolderPath + GCurrentData.projectName + ".fproj";
	//this->write(path.c_str());

	//GCurrentData.isDataChanged = false;
}

void FD::ProjectWrite::save_tab() const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lockTab(Internal::Coding::TabData::mtx);

	std::ofstream ofs(GCurrentData.projectFolderPath + "ProjectFiles/.tab", std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .tab file.");

	ofs << "DisplayFiles" << std::endl;
	for (const auto& x : TabData::displayFiles) {
		ofs << x << std::endl;
	}
	ofs << "Next" << std::endl;

	ofs << "FilePathes" << std::endl;
	for (const auto& x : TabData::filePathes) {
		ofs << x << std::endl;
	}
	ofs << "Next" << std::endl;

}

void FD::ProjectWrite::save_scene() const {
	using namespace Internal::Scene;

	std::lock_guard<std::mutex> lock(Data::mtx);

	std::ofstream ofs(Internal::Project::GCurrentData.projectFolderPath + "ProjectFiles/.scene", std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .scene file.");

	ofs << "SceneCodes" << std::endl;
	for (const auto& x : Data::codes) {
		ofs << x << std::endl;
	}
	ofs << "Next" << std::endl;

}

void FD::ProjectWrite::save_files() const {
	using namespace Internal::Project;

	std::ofstream ofs(GCurrentData.projectFolderPath + "ProjectFiles/.files", std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .files file.");

	ofs << "MainCodeFilePath" << std::endl;
	ofs << GFiles.mainCodeFilePath << std::endl;

	ofs << "CodeFilePathes" << std::endl;
	for (const auto& x : GFiles.includeCodeFilePathes) {
		ofs << x << std::endl;
	}
}

void FD::ProjectWrite::backup() const {
	using namespace Internal::Project;
	//assert(GCurrentData.projectFolderPath.back() == '/');

	//std::string path = Internal::Project::GCurrentData.projectFolderPath + "Backup/";
	//path += Internal::Project::getCurrentTime() += ".backup";

	//std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	////backupフォルダが消されていたら作る
	//this->tryCreateBackupFolder();

	//this->write(path.c_str());
}

void FD::ProjectWrite::setMainCodePath(const char* path) const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	GFiles.mainCodeFilePath = path;
}

void FD::ProjectWrite::addIncludeCodePath(const char* path) const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	GFiles.includeCodeFilePathes.emplace_back(path);
}

bool FD::ProjectWrite::eraseProjectHistory(const std::string& fprojPath) {
	using namespace Internal::Project;

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	std::vector<Project::HistoryInfo> temp(50);

	uint16_t index = std::numeric_limits<uint16_t>::max();

	//til 50
	for (uint16_t i = 0; i < 50; i++) {
		//projectname path time
		std::getline(ifs, temp[i].projectName);
		std::getline(ifs, temp[i].projectFilePath);
		std::getline(ifs, temp[i].ymd_h);

		if (temp[i].projectFilePath == fprojPath) {
			index = i;
		}
	}

	if (index == std::numeric_limits<uint16_t>::max())
		return false;

	temp.erase(temp.begin() + index);

	//書き直す
	std::ofstream ofs(Internal::Resource::RecentProjectHistoryFilePath, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to update FluidumProject history.");

	for (const auto& x : temp) {
		ofs << x.projectName << std::endl;
		ofs << x.projectFilePath << std::endl;
		ofs << x.ymd_h << std::endl;
	}
	return true;
}

void FD::ProjectWrite::writeProjectInfo(const char* path) const {
	std::ofstream ofs(path, std::ios::out | std::ios::binary);

	if (!ofs)
		throw std::runtime_error("Failed to write project file.");

	using namespace Internal::Project;

	//ullは64bit以上が約束されているので8byte固定で書き込む
	ofs.write(reinterpret_cast<const char*>(&FluidumProjectFileIdentifier), 8);
	ofs << std::endl;
	ofs << "Fluidum" << std::endl;

	ofs << "ProjectFilePath" << std::endl;
	ofs << GCurrentData.projectFilePath << std::endl;

	ofs << "Time" << std::endl;
	ofs << getCurrentTime() << std::endl;

	ofs << "ProjectName" << std::endl;
	ofs << GCurrentData.projectName << std::endl;

	ofs << "ProjectFolderPath" << std::endl;
	ofs << GCurrentData.projectFolderPath << std::endl;
}

void FD::ProjectWrite::tryCreateBackupFolder() const {
	using namespace Internal::Project;

	std::string backupFolderPath = GCurrentData.projectFolderPath;
	backupFolderPath += "ProjectFiles/Backup/";

	if (!std::filesystem::is_directory(backupFolderPath)) {
		if (!std::filesystem::create_directory(backupFolderPath))
			throw std::runtime_error("Failed to create Backup directory.");
	}
}

void FD::ProjectWrite::tryCreateSrcFolder() const {
	using namespace Internal::Project;

	std::string codeFolderPath = GCurrentData.projectFolderPath;
	codeFolderPath += "Src/";

	if (!std::filesystem::is_directory(codeFolderPath)) {
		if (!std::filesystem::create_directory(codeFolderPath))
			throw std::runtime_error("Failed to create Src directory.");
	}
}

void FD::ProjectWrite::tryCreateProjectFilesFolder() const {
	using namespace Internal::Project;

	std::string projectFilesPath = GCurrentData.projectFolderPath;
	projectFilesPath += "ProjectFiles/";

	if (!std::filesystem::is_directory(projectFilesPath)) {
		if (!std::filesystem::create_directory(projectFilesPath))
			throw std::runtime_error("Failed to create ProjectFiles directory.");
	}

#ifdef BOOST_OS_WINDOWS
	//set attribute
	const std::wstring wstr = FU::Text::utf8ToUtf16(projectFilesPath);
	int att = GetFileAttributes(wstr.data());
	if ((att & FILE_ATTRIBUTE_HIDDEN) == 0) {
		SetFileAttributes(wstr.data(), att | FILE_ATTRIBUTE_HIDDEN);
	}
#else
#error NotSupported
#endif
}

void FD::ProjectWrite::tryCreateFilesFile() const {
	using namespace Internal::Project;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath + "ProjectFiles/"))
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::string path = GCurrentData.projectFolderPath;
	path += "ProjectFiles/.files";

	if (std::filesystem::exists(path))
		return;

	std::ofstream ofs(path);

	if (!ofs)
		throw std::runtime_error("Failed to create .files file.");

}

void FD::ProjectWrite::tryCreateTabFile() const {
	using namespace Internal::Project;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath + "ProjectFiles/"))
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::string path = GCurrentData.projectFolderPath;
	path += "/ProjectFiles/.tab";

	if (std::filesystem::exists(path))
		return;

	std::ofstream ofs(path);

	if (!ofs)
		throw std::runtime_error("Failed to create .tab file.");

}

void FD::ProjectWrite::tryCreateSceneFile() const {
	using namespace Internal::Project;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath + "ProjectFiles/"))
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::string path = GCurrentData.projectFolderPath;
	path += "/ProjectFiles/.scene";

	if (std::filesystem::exists(path))
		return;

	std::ofstream ofs(path);

	if (!ofs)
		throw std::runtime_error("Failed to create .scene file.");

}

void FD::ProjectWrite::tryCreateFunctionFile() const {
	using namespace Internal::Project;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath + "ProjectFiles/"))
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::string path = GCurrentData.projectFolderPath;
	path += "/ProjectFiles/.func";

	if (std::filesystem::exists(path))
		return;

	std::ofstream ofs(path);

	if (!ofs)
		throw std::runtime_error("Failed to create .func file.");

}

void FD::ProjectWrite::checkIsProjectFolderExist() const {
	using namespace Internal::Project;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath))
		throw Project::ExceptionType::NotFoundProjectFolder;

}

void FD::ProjectWrite::checkIsProjectFileExist() const {
	using namespace Internal::Project;
	if (!std::filesystem::exists(GCurrentData.projectFilePath))
		throw Project::ExceptionType::NotFoundProjectFile;
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

	//いったんdequeに全て読み込む

	const std::string currentPath = GCurrentData.projectFolderPath + GCurrentData.projectName + ".fproj";
	uint16_t index = std::numeric_limits<uint16_t>::max();
	bool isNewFile = true;//新規ファイルで履歴にはない

	//100まで
	for (uint16_t i = 0; i < 50; i++) {
		//projectname path timeの順
		std::getline(ifs, temp[i].projectName);
		std::getline(ifs, temp[i].projectFilePath);
		std::getline(ifs, temp[i].ymd_h);

		if (temp[i].projectFilePath == currentPath) {
			index = i;
			isNewFile = false;
		}
	}

	//現在のファイルがあれば一番上にもっていく
	if (!isNewFile) {
		temp.emplace_front(temp.at(index));
		temp.erase(temp.begin() + index + 1);
	}
	//新規
	else {
		Project::HistoryInfo info;
		info.projectFilePath = currentPath;
		info.projectName = GCurrentData.projectName;
		info.ymd_h = getYMDH();
		temp.emplace_front(std::move(info));
	}


	//書き直す
	std::ofstream ofs(Internal::Resource::RecentProjectHistoryFilePath, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to update FluidumProject history.");

	for (const auto& x : temp) {
		ofs << x.projectName << std::endl;
		ofs << x.projectFilePath << std::endl;
		ofs << x.ymd_h << std::endl;
	}

}

void FD::ProjectWrite::readProjectInfo(std::ifstream& ifs) const {
	using namespace Internal::Project;
	using ExceptionType = ::FD::Project::ExceptionType;

	std::string data{};

	//Fluidum
	std::getline(ifs, data);
	std::getline(ifs, data);
	if (data != "Fluidum")
		throw ExceptionType::IllegalFile;

	//ProjectFilePath
	std::getline(ifs, data);
	if (data != "ProjectFilePath")
		throw ExceptionType::IllegalFile;
	std::getline(ifs, data);
	GCurrentData.projectFilePath = data;

	//Time
	std::getline(ifs, data);
	if (data != "Time")
		throw ExceptionType::IllegalFile;
	std::getline(ifs, data);

	//ProjectName
	std::getline(ifs, data);
	if (data != "ProjectName")
		throw ExceptionType::IllegalFile;
	std::getline(ifs, data);
	GCurrentData.projectName = data;

	//ProjectFolderPath
	std::getline(ifs, data);
	if (data != "ProjectFolderPath")
		throw ExceptionType::IllegalFile;
	std::getline(ifs, data);
	GCurrentData.projectFolderPath = data;

}

void FD::ProjectWrite::readProjectFiles() const {
	using namespace Internal::Project;

	std::ifstream ifs(GCurrentData.projectFolderPath + "ProjectFiles/.files");
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::string data{};

	//MainCodeFilePath
	std::getline(ifs, data);
	if (data != "MainCodeFilePath")
		throw Project::ExceptionType::IllegalFile;
	std::getline(ifs, data);
	GFiles.mainCodeFilePath = data;

	//IncludeFilePathes
	{
		std::getline(ifs, data);
		if (data != "CodeFilePathes")
			throw Project::ExceptionType::IllegalFile;
		std::size_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == "Next")
				break;
			GFiles.includeCodeFilePathes.emplace_back(data);

			counter++;
			if (counter > 10000)
				throw Project::ExceptionType::BrokenFile;
		}
	}

}

void FD::ProjectWrite::readTabInfo() const {
	using namespace Internal::Project;
	using namespace Internal::Coding;
	using ExceptionType = ::FD::Project::ExceptionType;

	std::ifstream ifs(GCurrentData.projectFolderPath + "ProjectFiles/.tab");
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::lock_guard<std::mutex> lock(Internal::Coding::TabData::mtx);

	std::string data{};

	//TabDisplayFile
	{
		std::getline(ifs, data);
		if (data != "DisplayFiles")
			throw ExceptionType::IllegalFile;

		uint16_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == "Next")
				break;
			Internal::Coding::TabData::displayFiles.emplace_back(data);

			counter++;
			if (counter > 1000)
				throw Project::ExceptionType::BrokenFile;
		}
	}

	//TabFilePathes
	{
		std::getline(ifs, data);
		if (data != "TabFilePathes")
			throw ExceptionType::IllegalFile;

		uint16_t counter = 0;
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
}

void FD::ProjectWrite::readSceneInfo() const {
	using namespace Internal::Project;

	using ExceptionType = ::FD::Project::ExceptionType;

	std::ifstream ifs(GCurrentData.projectFolderPath + "ProjectFiles/.scene");
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::lock_guard<std::mutex> lock(Internal::Scene::Data::mtx);

	std::string data{};

	//SceneCodes
	{
		std::getline(ifs, data);
		if (data != "SceneCodes")
			throw ExceptionType::IllegalFile;

		uint16_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == "Next")
				break;
			Internal::Scene::Data::codes.emplace_back(static_cast<FU::Class::ClassCode::CodeType>(std::stoul(data)));

			counter++;
			if (counter > 1000)
				throw Project::ExceptionType::BrokenFile;
		}
	}

}

std::array<FD::Project::HistoryInfo, 50> FD::ProjectRead::getProjectHistory() const {
	using namespace Internal::Project;
	std::lock_guard<std::mutex> lock(GMtx);

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	if (!ifs)
		throw std::runtime_error("Failed to load history file.");

	std::array<Project::HistoryInfo, 50> result{};

	//50まで
	for (std::size_t i = 0; i < std::tuple_size_v<decltype(result)>; i++) {
		//projectname path timeの順で入っている
		std::getline(ifs, result[i].projectName);
		std::getline(ifs, result[i].projectFilePath);
		std::getline(ifs, result[i].ymd_h);
	}

	return result;
}

bool FD::ProjectRead::isDataChanged() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return false;
	//eturn Internal::Project::GCurrentData.isDataChanged;
}

std::vector<std::string> FD::ProjectRead::getIncludeCodeFilePathes() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GFiles.includeCodeFilePathes;
}

std::string FD::ProjectRead::getMainCodeFilePath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GFiles.mainCodeFilePath;
}

bool FD::ProjectRead::isMainCodeFileExist() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GFiles.mainCodeFilePath.empty();
}

std::string FD::ProjectRead::getProjectFolderPath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectFolderPath;
}

std::string FD::ProjectRead::getBackupFolderPath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectFolderPath + "Backup/";
}

std::string FD::ProjectRead::getSrcFolderPath() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectFolderPath + "Src/";
}

std::string FD::ProjectRead::getProjectName() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.projectName;
}

bool FD::ProjectRead::isDefaultProject() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	return Internal::Project::GCurrentData.isTemp;
}

FD::Project::CodeType FD::ProjectRead::getCurrentMainCodeType() const {
	using namespace Internal::Project;

	if (GFiles.mainCodeFilePath.empty())
		return Project::CodeType::Empty;

	std::string extension{};
	(void)std::find_if(
		GFiles.mainCodeFilePath.rbegin(),
		GFiles.mainCodeFilePath.rend(),
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

std::vector<FU::Class::ClassCode::CodeType> FD::ProjectRead::loadSceneFile() const {
	std::lock_guard<std::mutex> lock(Internal::Project::GMtx);
	std::lock_guard<std::mutex> lockScene(Internal::Scene::Data::mtx);

	std::ifstream ifs(Internal::Project::GCurrentData.projectFolderPath + "ProjectFiles/.scene");

	std::string data{};
	std::getline(ifs, data);
	if (data != "SceneCodes")
		abort();

	std::vector<FU::Class::ClassCode::CodeType> result{};
	while(true) {
		std::getline(ifs, data);
		if (data == "Next")
			break;
		result.emplace_back(static_cast<FU::Class::ClassCode::CodeType>(std::stoll(data)));
	};

	return result;
}
