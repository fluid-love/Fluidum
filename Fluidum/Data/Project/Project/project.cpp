#include "project.h"
#include "../../Coding/tab.h"
#include "../../Scene/scene.h"
#include "../Files/files.h"
#include "../../Gui/layout.h"
#include "name.h"

namespace FD::Project::Internal {

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

}

void FD::ProjectWrite::save_thread() {

	bool request = this->saveThread.get_stop_token().stop_requested();

	while (!request) {
		request = this->saveThread.get_stop_token().stop_requested();
		if (Internal::Coding::TabData::save) {
			Internal::Coding::TabData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			this->save_tab();
		}
		if (Internal::Scene::Data::save) {
			Internal::Scene::Data::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			this->save_scene();
		}
		if (Project::Internal::FluidumFilesData::save) {
			Project::Internal::FluidumFilesData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			this->save_fluidumFiles();
		}
		if (Project::Internal::ProjectFilesData::save) {
			Project::Internal::ProjectFilesData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			this->save_projectFiles();
		}
		if (Project::Internal::UserFilesData::save) {
			Project::Internal::UserFilesData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			this->save_userFiles();
		}
		if (Layout::Internal::LayoutData::save) {
			Layout::Internal::LayoutData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			this->save_layout();
		}
	}

}

namespace FD::Project::Internal {

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

}

//Constructor
FD::ProjectWrite::ProjectWrite(Internal::PassKey) {
	using namespace Project::Internal;

	//remove last temp project.
	//create new temp project.
	std::filesystem::remove_all(Internal::Resource::TempProjectFolderPath);
	if (!std::filesystem::create_directory(Internal::Resource::TempProjectFolderPath))
		throw std::runtime_error("Failed to create TempProject directory.");

	//temp project name -> current time
	std::string time = getCurrentTime();
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

	this->tryCreateFluidumFolder();
	this->tryCreateFluidumFiles();

	this->writeProjectInfo(path.c_str());
}

void FD::ProjectWrite::createNewProject(const Project::CreateProjectInfo& info) const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);

	Data temp{};

	try {
		temp = std::move(GCurrentData);
	}
	catch (...) {
		throw Project::ExceptionType::Unexpected;
	}

	try {
		GCurrentData.reset();

		GCurrentData.projectFolderPath = FU::File::consistentDirectory(info.projectFolderPath);
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
		this->tryCreateFluidumFolder();
		this->tryCreateFluidumFiles();


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
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(GMtx);


	//例外がでた場合にもとの情報に戻す
	Data temp{};

	//files
	std::string temp_files_mainCodeFilePath{};
	Project::Internal::FileList temp_files_projectFiles{};
	Project::Internal::FileList temp_files_userFiles{};

	//tab
	std::vector<std::string> temp_tab_filePathes{};
	std::vector<std::string> temp_tab_displayFiles{};

	//scene
	std::vector<FU::Class::ClassCode::CodeType> temp_scene_codes{};

	//layout
	std::vector<Layout::Internal::History> temp_layout_history{};

	try {
		temp = GCurrentData;
		temp_files_mainCodeFilePath = Project::Internal::FluidumFilesData::mainCodeFilePath;
		temp_files_projectFiles = Project::Internal::ProjectFilesData::projectFiles;
		temp_files_userFiles = Project::Internal::UserFilesData::userFiles;
		temp_tab_displayFiles = Internal::Coding::TabData::displayFiles;
		temp_tab_filePathes = Internal::Coding::TabData::filePathes;
		temp_scene_codes = Internal::Scene::Data::codes;
		temp_layout_history = Layout::Internal::LayoutData::history;
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

		//Project
		this->read_fluidumFiles();
		this->read_projectFiles();
		this->read_userFiles();

		this->read_layout();

		this->read_tab();
		this->read_scene();

		this->updateHistory();
	}
	catch (...) {
		GCurrentData = std::move(temp);

		Project::Internal::FluidumFilesData::mainCodeFilePath = std::move(temp_files_mainCodeFilePath);
		Project::Internal::ProjectFilesData::projectFiles = std::move(temp_files_userFiles);
		Project::Internal::UserFilesData::userFiles = std::move(temp_files_projectFiles);
		Internal::Coding::TabData::displayFiles = std::move(temp_tab_displayFiles);
		Internal::Coding::TabData::filePathes = std::move(temp_tab_filePathes);
		Internal::Scene::Data::codes = std::move(temp_scene_codes);
		Layout::Internal::LayoutData::history = std::move(temp_layout_history);
		std::rethrow_exception(std::current_exception());
	}
	GCurrentData.isTemp = false;
}

void FD::ProjectWrite::saveAs(const char* newName, const char* dstProjectFolderPath) const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);

	std::string projectFolderPath = FU::File::consistentDirectory(dstProjectFolderPath);

	//まず，コピー先に同じ名前のフォルダがあるかチェック
	if (!std::filesystem::is_directory(projectFolderPath))
		throw Project::ExceptionType::AlreadyProjectFolderExist;

	//現在のフォルダが移動や削除されていないか
	this->checkIsProjectFolderExist();
	this->checkIsProjectFileExist();

	using namespace Project::Internal;

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
		std::string srcPath = GCurrentData.projectFolderPath + Name::FluidumHiddenFolder;

		//set new folder path
		GCurrentData.projectFolderPath = projectFolderPath + newName + '/';

		std::filesystem::copy(srcPath, GCurrentData.projectFolderPath + Name::FluidumHiddenFolder, std::filesystem::copy_options::recursive);

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

void FD::ProjectWrite::save_tab() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lockTab(Internal::Coding::TabData::mtx);

	std::ofstream ofs(GCurrentData.projectFolderPath + Name::Project_Tab, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .tab file.");

	//DisplayFiles
	for (const auto& x : Internal::Coding::TabData::displayFiles) {
		ofs << x << std::endl;
	}
	ofs << Name::Delimiter << std::endl;

	//TabFilePathes
	for (const auto& x : Internal::Coding::TabData::filePathes) {
		ofs << x << std::endl;
	}
	ofs << Name::Delimiter << std::endl;

}

void FD::ProjectWrite::save_scene() const {
	using namespace Internal::Scene;

	std::lock_guard<std::mutex> lock(Data::mtx);

	std::ofstream ofs(Project::Internal::GCurrentData.projectFolderPath + Project::Internal::Name::Project_Scene, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .scene file.");

	for (const auto& x : Data::codes) {
		ofs << x << std::endl;
	}
	ofs << Project::Internal::Name::Delimiter << std::endl;

}

void FD::ProjectWrite::save_files_recursive(std::ofstream& ofs, const Project::List::FileInfo* info) const {
	ofs << info->path << std::endl;
	ofs << info->name << std::endl;
	ofs << info->open << std::endl;
	ofs << static_cast<std::underlying_type_t<decltype(info->type)>>(info->type) << std::endl;

	ofs << info->dir_internal.size() << std::endl;
	for (std::size_t i = 0, size = info->dir_internal.size(); i < size; i++) {
		save_files_recursive(ofs, &info->dir_internal[i]);
	}
}

void FD::ProjectWrite::save_fluidumFiles() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);

	std::ofstream ofs(GCurrentData.projectFolderPath + Name::Project_FluidumFiles, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .fluidumfiles file.");

	ofs << FluidumFilesData::mainCodeFilePath << std::endl;

	ofs << Name::Delimiter << std::endl;
}

void FD::ProjectWrite::save_projectFiles() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(ProjectFilesData::mtx);

	std::ofstream ofs(GCurrentData.projectFolderPath + Name::Project_ProjectFiles, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .projectfiles file.");

	auto data = Project::Internal::ProjectFilesData::projectFiles.get();
	for (auto& x : *data) {
		this->save_files_recursive(ofs, &x);
	}

	ofs << Name::Delimiter << std::endl;
}

void FD::ProjectWrite::save_userFiles() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(UserFilesData::mtx);

	std::ofstream ofs(GCurrentData.projectFolderPath + Name::Project_UserFiles, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .userfiles file.");

	auto data = Project::Internal::UserFilesData::userFiles.get();
	for (auto& x : *data) {
		this->save_files_recursive(ofs, &x);
	}

	ofs << Name::Delimiter << std::endl;
}

void FD::ProjectWrite::save_layout() const {
	using namespace Layout::Internal;

	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	std::ofstream ofs(
		Project::Internal::GCurrentData.projectFolderPath + Project::Internal::Name::Project_Layout,
		std::ios::trunc
	);

	if (!ofs)
		throw std::runtime_error("Failed to open .layout file.");

	ofs << LayoutData::mainFrameLeft << std::endl;
	ofs << LayoutData::mainFrameRight << std::endl;
	ofs << LayoutData::mainFrameTop << std::endl;
	ofs << LayoutData::mainFrameBottom << std::endl;

	for (auto& x : LayoutData::history) {
		if (x.horizonal)
			ofs << "true" << std::endl;
		else
			ofs << "false" << std::endl;

		ofs << x.parentWindowIndex << std::endl;
		ofs << *x.pos << std::endl;
	}

	ofs << Project::Internal::Name::Delimiter << std::endl;
}

void FD::ProjectWrite::backup() const {
	using namespace Project::Internal;
	//assert(GCurrentData.projectFolderPath.back() == '/');

	//std::string path = Internal::Project::GCurrentData.projectFolderPath + "Backup/";
	//path += Internal::Project::getCurrentTime() += ".backup";

	//std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	////backupフォルダが消されていたら作る
	//this->tryCreateBackupFolder();

	//this->write(path.c_str());
}

bool FD::ProjectWrite::eraseProjectHistory(const std::string& fprojPath) {
	using namespace Project::Internal;

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

	using namespace Project::Internal;

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

void FD::ProjectWrite::tryCreateFluidumFolder() const {
	using namespace Project::Internal;

	constexpr const char* paths[] = {
		Name::FluidumHiddenFolder,
		Name::ProjectFolder,
		Name::GenomeFolder,
		Name::BackupFolder,
		Name::SrcFolder
	};

	for (auto x : paths) {
		std::string path = GCurrentData.projectFolderPath + x;
		if (!std::filesystem::is_directory(path)) {
			if (!std::filesystem::create_directory(path))
				throw std::runtime_error("Failed to create directory.");
		}
	}

	//hide
	std::string path = GCurrentData.projectFolderPath + paths[0];
	FU::File::hide(path);
}

void FD::ProjectWrite::tryCreateFluidumFiles() const {
	using namespace Project::Internal;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath + Name::FluidumHiddenFolder))
		throw Project::ExceptionType::NotFoundProjectFiles;

	constexpr const char* paths[] = {
		Name::Project_FluidumFiles,
		Name::Project_ProjectFiles,
		Name::Project_UserFiles,
		Name::Project_Tab,
		Name::Project_Scene,
		Name::Project_Layout,
		Name::Genome_Function
	};

	for (auto x : paths) {
		std::string path = GCurrentData.projectFolderPath + x;

		if (std::filesystem::exists(path))
			continue;

		std::ofstream ofs(path);
		if (!ofs)
			throw std::runtime_error("Failed to create file.");
	}

}

void FD::ProjectWrite::checkIsProjectFolderExist() const {
	using namespace Project::Internal;

	if (!std::filesystem::is_directory(GCurrentData.projectFolderPath))
		throw Project::ExceptionType::NotFoundProjectFolder;

}

void FD::ProjectWrite::checkIsProjectFileExist() const {
	using namespace Project::Internal;
	if (!std::filesystem::exists(GCurrentData.projectFilePath))
		throw Project::ExceptionType::NotFoundProjectFile;
}

namespace FD::Project::Internal {
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
	using namespace Project::Internal;

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
	using namespace Project::Internal;
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

FD::Project::List::FileInfo FD::ProjectWrite::readProjectFiles_element(std::ifstream& ifs) const {
	std::string data{};

	Project::List::FileInfo info;

	std::getline(ifs, data);
	info.path = data;

	std::getline(ifs, data);
	info.name = data;

	std::getline(ifs, data);
	if (static_cast<bool>(std::stoi(data)) == true)
		info.open = true;
	else
		info.open = false;

	std::getline(ifs, data);
	using UT = std::underlying_type_t<decltype(info.type)>;
	using Type = Project::Internal::FileList::Type;
	UT type = std::stoi(data);
	if (type == static_cast<UT>(Type::Directory))
		info.type = Type::Directory;
	else if (type == static_cast<UT>(Type::Code))
		info.type = Type::Code;
	else if (type == static_cast<UT>(Type::Unsupported))
		info.type = Type::Unsupported;

	return info;
}

void FD::ProjectWrite::read_files_recursive(std::ifstream& ifs, Project::List::FileInfo* parent) const {
	std::string data{};

	parent->dir_internal.emplace_back(this->readProjectFiles_element(ifs));

	//size
	std::getline(ifs, data);
	std::size_t size = std::stoull(data);
	for (std::size_t i = 0; i < size; i++) {
		this->read_files_recursive(ifs, &parent->dir_internal.back());
	}
}

void FD::ProjectWrite::read_fluidumFiles() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);

	std::ifstream ifs(GCurrentData.projectFolderPath + Name::Project_FluidumFiles);
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	//MainCodeFilePath
	std::getline(ifs, buf);

	FluidumFilesData::mainCodeFilePath = buf;
}

void FD::ProjectWrite::read_projectFiles() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(ProjectFilesData::mtx);

	std::ifstream ifs(GCurrentData.projectFolderPath + Name::Project_ProjectFiles);
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	std::size_t counter = 0;
	while (true) {
		std::getline(ifs, buf);
		if (buf == Name::Delimiter)
			break;

		auto data = UserFilesData::userFiles.get();
		auto info = this->readProjectFiles_element(ifs);
		data->emplace_back(std::move(info));
		this->read_files_recursive(ifs, &info);

		counter++;
		if (counter > 1000)
			throw Project::ExceptionType::BrokenFile;
	}
}

void FD::ProjectWrite::read_userFiles() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(UserFilesData::mtx);

	std::ifstream ifs(GCurrentData.projectFolderPath + Name::Project_UserFiles);
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	std::size_t counter = 0;
	while (true) {
		std::getline(ifs, buf);
		if (buf == Name::Delimiter)
			break;

		auto data = UserFilesData::userFiles.get();
		auto info = this->readProjectFiles_element(ifs);
		data->emplace_back(std::move(info));
		this->read_files_recursive(ifs, &info);

		counter++;
		if (counter > 1000)
			throw Project::ExceptionType::BrokenFile;
	}
}

void FD::ProjectWrite::read_layout() const {
	using namespace Project::Internal;
	using namespace Layout::Internal;

	std::unique_lock<std::mutex> lock(LayoutData::mtx);

	std::ifstream ifs(GCurrentData.projectFolderPath + Name::Project_Layout);
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	//main frame
	try {
		std::getline(ifs, buf);
		LayoutData::mainFrameLeft = std::stof(buf);
		std::getline(ifs, buf);
		LayoutData::mainFrameRight = std::stof(buf);
		std::getline(ifs, buf);
		LayoutData::mainFrameTop = std::stof(buf);
		std::getline(ifs, buf);
		LayoutData::mainFrameBottom = std::stof(buf);
	}
	catch (const std::exception&) {
		throw Project::ExceptionType::BrokenFile;
	}

	std::size_t counter = 0;
	while (true) {
		std::getline(ifs, buf);
		if (buf == Name::Delimiter)
			break;

		Layout::Internal::History his{};

		if (buf == "true")
			his.horizonal = true;
		else if (buf == "false")
			his.horizonal = false;
		else
			throw Project::ExceptionType::BrokenFile;

		try {
			std::getline(ifs, buf);
			his.parentWindowIndex = static_cast<uint32_t>(std::stoul(buf));
			std::getline(ifs, buf);
			his.readPos = std::stof(buf);
		}
		catch (const std::exception&) {
			throw Project::ExceptionType::BrokenFile;
		}


		LayoutData::history.emplace_back(his);


		counter++;
		if (counter > 200)
			throw Project::ExceptionType::BrokenFile;
	}

	lock.unlock();
	LayoutData::remake();
}

void FD::ProjectWrite::read_tab() const {
	using namespace Project::Internal;

	using ExceptionType = ::FD::Project::ExceptionType;

	std::lock_guard<std::mutex> lock(Internal::Coding::TabData::mtx);

	std::ifstream ifs(GCurrentData.projectFolderPath + Name::Project_Tab);
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	std::string data{};

	if (FU::File::empty(ifs))
		return;

	//TabDisplayFile
	{
		uint16_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == Name::Delimiter)
				break;
			Internal::Coding::TabData::displayFiles.emplace_back(data);

			counter++;
			if (counter > 1000)
				throw Project::ExceptionType::BrokenFile;
		}
	}

	//TabFilePathes
	{
		uint16_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == Name::Delimiter)
				break;
			Internal::Coding::TabData::filePathes.emplace_back(data);

			counter++;
			if (counter > 1000)
				throw Project::ExceptionType::BrokenFile;
		}
	}
}

void FD::ProjectWrite::read_scene() const {
	using namespace Project::Internal;

	using ExceptionType = ::FD::Project::ExceptionType;

	std::lock_guard<std::mutex> lock(Internal::Scene::Data::mtx);

	std::ifstream ifs(GCurrentData.projectFolderPath + Name::Project_Scene);
	if (!ifs)
		throw Project::ExceptionType::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string data{};

	//SceneCodes
	{
		uint16_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == Name::Delimiter)
				break;
			Internal::Scene::Data::codes.emplace_back(static_cast<FU::Class::ClassCode::CodeType>(std::stoul(data)));

			counter++;
			if (counter > 1000)
				throw Project::ExceptionType::BrokenFile;
		}
	}

}

std::array<FD::Project::HistoryInfo, 50> FD::ProjectRead::getProjectHistory() const {
	using namespace Project::Internal;

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
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return false;
	//eturn Internal::Project::GCurrentData.isDataChanged;
}

std::string FD::ProjectRead::getProjectFolderPath() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectFolderPath;
}

std::string FD::ProjectRead::getBackupFolderPath() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectFolderPath + "Backup/";
}

std::string FD::ProjectRead::getSrcFolderPath() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectFolderPath + "Src/";
}

std::string FD::ProjectRead::getProjectName() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectName;
}

bool FD::ProjectRead::isDefaultProject() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.isTemp;
}

std::vector<FU::Class::ClassCode::CodeType> FD::ProjectRead::loadSceneFile() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	std::lock_guard<std::mutex> lockScene(Internal::Scene::Data::mtx);

	std::ifstream ifs(Project::Internal::GCurrentData.projectFolderPath + Project::Internal::Name::Project_Scene);

	std::string data{};

	std::vector<FU::Class::ClassCode::CodeType> result{};
	while (true) {
		std::getline(ifs, data);
		if (data == Project::Internal::Name::Delimiter)
			break;
		result.emplace_back(static_cast<FU::Class::ClassCode::CodeType>(std::stoll(data)));
	};

	return result;
}
