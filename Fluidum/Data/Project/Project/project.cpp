#include "project.h"
#include "../../Coding/tab.h"
#include "../../Scene/scene.h"
#include "../Files/files.h"
#include "../../Gui/layout.h"
#include "../../../Scene/Utils/Scene/classcodes.h"
#include "name.h"

namespace FD::Project::Internal {

	//current
	struct Data final {
	public:
		std::string projectName{};
		std::string projectFilePath{};
		std::string projectDirectoryPath{};

	public:
		bool isTempProject = true;

	public:
		void reset() {
			*this = Data();
		}

	}GCurrentData;

	std::mutex GMtx{};

	constexpr UI64 FluidumProjectFileIdentifier =
		0b01110100'11011001'01100111'01011000'01010000'10110101'10011000'00111000;

}

void FD::ProjectWrite::save_thread() {

	bool request = this->saveThread.get_stop_token().stop_requested();

	while (!request) {
		request = this->saveThread.get_stop_token().stop_requested();
		if (Coding::Internal::Data::save) {
			Coding::Internal::Data::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockTab(Coding::Internal::Data::mtx);
			this->save_tab();
		}
		if (Scene::Internal::Data::save) {
			Scene::Internal::Data::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockScene(::FD::Scene::Internal::Data::mtx);
			this->save_scene();
		}
		if (Project::Internal::FluidumFilesData::save) {
			Project::Internal::FluidumFilesData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockFiles(Project::Internal::FluidumFilesData::mtx);
			this->save_fluidumFiles();
		}
		if (Project::Internal::ProjectFilesData::save) {
			Project::Internal::ProjectFilesData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockFiles(Project::Internal::ProjectFilesData::mtx);
			this->save_projectFiles();
		}
		if (Project::Internal::UserFilesData::save) {
			Project::Internal::UserFilesData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockFiles(Project::Internal::UserFilesData::mtx);
			this->save_userFiles();
		}
		if (Layout::Internal::LayoutData::save) {
			Layout::Internal::LayoutData::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockLayout(Layout::Internal::LayoutData::mtx);
			this->save_layout();
		}
	}

}

namespace FD::Project::Internal {

	[[nodiscard]] std::string getCurrentTime() {
		const auto zone = std::chrono::current_zone()->name();
		const auto time1 = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time();
		const auto time2 = std::chrono::floor<std::chrono::days>(time1);
		std::chrono::year_month_day ymd(time2);

		std::chrono::hh_mm_ss time(std::chrono::floor< std::chrono::milliseconds>(time1 - time2));

		std::stringstream ss;
		ss << zone << '_' << ymd.year() << '_' << ymd.month() << '_' << ymd.day() << '_'
			<< time.hours().count() << '_' << time.minutes().count() << '_' << time.seconds().count() << '_' << time.subseconds().count();
		return ss.str();
	}

}

//Constructor
FD::ProjectWrite::ProjectWrite(Internal::PassKey) noexcept {
	using namespace Project::Internal;

	try {
		//remove previous temp project.
		//create new temp project.
		std::filesystem::remove_all(Internal::Resource::TempProjectFolderPath);
		if (!std::filesystem::create_directory(Internal::Resource::TempProjectFolderPath))
			throw - 1;

		//temp project name -> current time
		const std::string time = getCurrentTime();
		const std::string projectName = time;

		//path -> Fluidum/Project/TempProject
		std::string path = Internal::Resource::TempProjectFolderPath;
		GCurrentData.projectDirectoryPath = path;

		(path += time) += ".fproj";
		GCurrentData.projectFilePath = path;

		std::ofstream ofs(path, std::ios::out);
		if (!ofs)
			throw - 1;

		GCurrentData.projectName = projectName;

		this->tryCreateFluidumDirectory();
		this->tryCreateFluidumFiles();

		this->writeProjectInfo(path.c_str());
	}
	catch (...) {
		try {
			std::cerr << "[Serious Error] Failed to construct ::FD::ProjectWrite.";
		}
		catch (...) {
			;
		}
		std::abort();
	}
}

void FD::ProjectWrite::createNewProject(const CreateInfo& info) {
	using namespace Project::Internal;

	//system_error
	std::unique_lock<std::mutex> lock(Project::Internal::GMtx);

	//no-throw
	//std::string is still valid after move.
	Data temp = std::move(GCurrentData);
	static_assert(FU::Concept::IsNothrowMoveConstructibleAssignable<Data>);

	try {
		GCurrentData.reset();		
		GCurrentData.projectDirectoryPath = FU::File::consistentDirectory(info.projectDirectoryPath);

		assert(FU::File::isAbsolute(GCurrentData.projectDirectoryPath));

		GCurrentData.projectName = info.projectName;

		//check
		if (GCurrentData.projectDirectoryPath.back() != '/')
			GCurrentData.projectDirectoryPath.push_back('/');
		GCurrentData.projectDirectoryPath += (GCurrentData.projectName + '/');

		//create ProjectDirectory
		if (std::filesystem::is_directory(GCurrentData.projectDirectoryPath))
			throw Exception::AlreadyProjectDirctoryExist;
	}
	catch (const Exception) {
		GCurrentData = std::move(temp);
		FU::Exception::rethrow();
	}
	catch (...) {
		GCurrentData = std::move(temp);
		throw Exception::Unexpected;
	}

	//directory
	try {
		if (!std::filesystem::create_directory(GCurrentData.projectDirectoryPath))
			throw - 1;
	}
	catch (...) {
		GCurrentData = std::move(temp);
		throw Exception::Unexpected;
	}

	try {
		this->tryCreateFluidumDirectory();
		this->tryCreateFluidumFiles();


		//.fproj
		std::string path = GCurrentData.projectDirectoryPath;
		path += (GCurrentData.projectName + ".fproj");
		GCurrentData.projectFilePath = path;
		this->writeProjectInfo(path.c_str());
		this->updateHistory();
	}
	catch (const Exception) {
		GCurrentData = std::move(temp);
		std::rethrow_exception(std::current_exception());
	}
	catch (...) {
		//remove project directory
		try {
			std::filesystem::remove_all(GCurrentData.projectDirectoryPath);
		}
		catch (...) {
			GCurrentData = std::move(temp);
			try { lock.unlock(); }
			catch (...) { FU::Exception::terminate(FU::Exception::TerminateExceptionType::Project); }
			std::terminate();
		}
		GCurrentData = std::move(temp);
		throw Exception::Unexpected;
	}

	GCurrentData.isTempProject = false;
}

void FD::ProjectWrite::loadProject(const std::string& path) {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(GMtx);

	Data temp{};

	//files
	std::string temp_files_mainCodeFilePath{};
	Project::Internal::FileList temp_files_projectFiles{};
	Project::Internal::FileList temp_files_userFiles{};

	//tab
	std::vector<std::string> temp_tab_filePaths{};
	std::vector<std::string> temp_tab_displayFiles{};

	//scene
	std::vector<FU::Class::ClassCode::CodeType> temp_scene_codes{};

	//layout
	std::vector<Layout::Internal::History> temp_layout_history{};

	std::lock_guard<std::mutex> lock1(FluidumFilesData::mtx);
	std::lock_guard<std::mutex> lock2(ProjectFilesData::mtx);
	std::lock_guard<std::mutex> lock3(UserFilesData::mtx);
	std::lock_guard<std::mutex> lock4(Layout::Internal::LayoutData::mtx);
	std::lock_guard<std::mutex> lock5(Coding::Internal::Data::mtx);
	std::lock_guard<std::mutex> lock6(Scene::Internal::Data::mtx);

	try {//copy
		temp = GCurrentData;
		temp_files_mainCodeFilePath = Project::Internal::FluidumFilesData::mainCodeFilePath;
		temp_files_projectFiles = Project::Internal::ProjectFilesData::projectFiles;
		temp_files_userFiles = Project::Internal::UserFilesData::userFiles;
		temp_layout_history = Layout::Internal::LayoutData::history;
		temp_tab_displayFiles = Coding::Internal::Data::displayFiles;
		temp_tab_filePaths = Coding::Internal::Data::filePaths;
		temp_scene_codes = Scene::Internal::Data::codes;
	}
	catch (...) {
		throw Exception::Unexpected;
	}

	std::ifstream ifs(path, std::ios::in | std::ios::binary);
	if (!ifs) {
		throw Exception::FailedToOpenProjectFile;
	}

	try {
		UI64 identifier = 0;
		ifs.read(reinterpret_cast<char*>(&identifier), 8);//identifier

		if (identifier != FluidumProjectFileIdentifier)
			throw Exception::IllegalFile;

		this->readProjectInfo(ifs);
		this->checkIsProjectFolderExist();

		//Project
		this->read_fluidumFiles();
		this->read_projectFiles();
		this->read_userFiles();

		this->read_layout();

		this->read_tab();
		this->read_scene();

		this->updateHistory();
	}
	catch (const Exception) {
		//no-throw

		GCurrentData = std::move(temp);

		Project::Internal::FluidumFilesData::mainCodeFilePath = std::move(temp_files_mainCodeFilePath);
		Project::Internal::ProjectFilesData::projectFiles = std::move(temp_files_userFiles);
		Project::Internal::UserFilesData::userFiles = std::move(temp_files_projectFiles);
		Coding::Internal::Data::displayFiles = std::move(temp_tab_displayFiles);
		Coding::Internal::Data::filePaths = std::move(temp_tab_filePaths);
		Scene::Internal::Data::codes = std::move(temp_scene_codes);

		Layout::Internal::LayoutData::history = std::move(temp_layout_history);

		std::rethrow_exception(std::current_exception());
	}
	catch (...) {
		GCurrentData = std::move(temp);

		Project::Internal::FluidumFilesData::mainCodeFilePath = std::move(temp_files_mainCodeFilePath);
		Project::Internal::ProjectFilesData::projectFiles = std::move(temp_files_userFiles);
		Project::Internal::UserFilesData::userFiles = std::move(temp_files_projectFiles);
		Coding::Internal::Data::displayFiles = std::move(temp_tab_displayFiles);
		Coding::Internal::Data::filePaths = std::move(temp_tab_filePaths);
		Scene::Internal::Data::codes = std::move(temp_scene_codes);

		Layout::Internal::LayoutData::history = std::move(temp_layout_history);

		throw Exception::Unexpected;
	}

	//make layout
	FD::Layout::Internal::LayoutData::remake();

	GCurrentData.isTempProject = false;
}

void FD::ProjectWrite::saveAs(const std::string& newName, const std::string& dstProjectDirectoryPath) {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);

	const std::string projectDirectoryPath = FU::File::consistentDirectory(dstProjectDirectoryPath);
	const std::string fullPath = projectDirectoryPath + newName;

	//First, check if there is a folder with the same name in the destination folder.
	if (!std::filesystem::is_directory(projectDirectoryPath))
		throw Exception::NotFoundDirectory;
	if (std::filesystem::is_directory(projectDirectoryPath))
		throw Exception::AlreadyExist;

	//Check that the current folder has not been moved or deleted.
	this->checkIsProjectFolderExist();
	this->checkIsProjectFileExist();

	using namespace Project::Internal;

	//create project folder
	if (!std::filesystem::create_directory(fullPath))
		throw Exception::Unexpected;

	Data temp{};

	//copy
	try { temp = GCurrentData; }
	catch (...) { throw Exception::Unexpected; }

	//copy
	try {
		const std::string srcPath = GCurrentData.projectDirectoryPath + Name::FluidumHiddenDirectory;

		//set new folder path
		GCurrentData.projectDirectoryPath = fullPath + '/';

		std::filesystem::copy(srcPath, GCurrentData.projectDirectoryPath + Name::FluidumHiddenDirectory, std::filesystem::copy_options::recursive);

		GCurrentData.projectName = newName;
		GCurrentData.projectFilePath = GCurrentData.projectDirectoryPath + newName + ".fproj";

		this->updateHistory();
	}
	catch (...) {
		std::filesystem::remove(GCurrentData.projectDirectoryPath);
		GCurrentData = temp;
		std::rethrow_exception(std::current_exception());
	}

}

void FD::ProjectWrite::save_tab() {
	using namespace Project::Internal;

	//already locked

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_Tab, std::ios::trunc);

		if (!ofs)
			throw std::runtime_error("Failed to open .tab file.");

		//DisplayFiles
		for (const auto& x : Coding::Internal::Data::displayFiles) {
			ofs << x << std::endl;
		}
		ofs << Name::Delimiter << std::endl;

		//TabFilePathes
		for (const auto& x : Coding::Internal::Data::filePaths) {
			ofs << x << std::endl;
		}
		ofs << Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_Tab, //old
			GCurrentData.projectDirectoryPath + Name::Project_Tab       //new
		);
	}
}

void FD::ProjectWrite::save_scene() {
	using namespace Project::Internal;

	//already locked 

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_Scene, std::ios::trunc);

		if (!ofs)
			throw std::runtime_error("Failed to open .scene file.");

		for (const auto& x : ::FD::Scene::Internal::Data::codes) {
			const auto find = std::find(std::begin(::FS::Utils::Class::ClassCodesView), std::end(::FS::Utils::Class::ClassCodesView), x);
			if (find != std::end(::FS::Utils::Class::ClassCodesView)) {
				const auto distance = std::distance(std::begin(::FS::Utils::Class::ClassCodesView), find);
				ofs << ::FS::Utils::Class::ClassCodesViewNames[distance] << std::endl;
			}
		}
		ofs << Project::Internal::Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_Scene, //old
			GCurrentData.projectDirectoryPath + Name::Project_Scene       //new
		);
	}
}

void FD::ProjectWrite::save_files_recursive(std::ofstream& ofs, const Project::FileList::FileInfo* info) {
	ofs << info->path << std::endl;
	ofs << info->name << std::endl;
	ofs << info->open << std::endl;

	if (info->type == FD::Project::FileList::Type::Directory)
		ofs << "D" << std::endl;
	else if (info->type == FD::Project::FileList::Type::Supported)
		ofs << "S" << std::endl;
	else {
		assert(info->type == FD::Project::FileList::Type::Unsupported);
		ofs << "U" << std::endl;
	}

	ofs << info->dir_internal.size() << std::endl;

	for (Size i = 0, size = info->dir_internal.size(); i < size; i++) {
		save_files_recursive(ofs, &info->dir_internal[i]);
	}
}

void FD::ProjectWrite::save_fluidumFiles() {
	using namespace Project::Internal;

	//already locked

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_FluidumFiles, std::ios::trunc);

		if (!ofs)
			throw std::runtime_error("Failed to open .fluidumfiles file.");

		ofs << FluidumFilesData::mainCodeFilePath << std::endl;

		ofs << Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_FluidumFiles, //old
			GCurrentData.projectDirectoryPath + Name::Project_FluidumFiles       //new
		);
	}
}

void FD::ProjectWrite::save_projectFiles() {
	using namespace Project::Internal;

	//already locked

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_ProjectFiles, std::ios::trunc);

		if (!ofs)
			throw std::runtime_error("Failed to open .projectfiles file.");

		auto data = Project::Internal::ProjectFilesData::projectFiles.get();
		for (auto& x : *data) {
			this->save_files_recursive(ofs, &x);
		}

		ofs << Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_ProjectFiles, //old
			GCurrentData.projectDirectoryPath + Name::Project_ProjectFiles       //new
		);
	}
}

void FD::ProjectWrite::save_userFiles() {
	using namespace Project::Internal;

	//already locked

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_UserFiles, std::ios::trunc);

		if (!ofs)
			throw std::runtime_error("Failed to open .userfiles file.");

		auto data = Project::Internal::UserFilesData::userFiles.get();
		for (auto& x : *data) {
			this->save_files_recursive(ofs, &x);
		}

		ofs << Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_UserFiles, //old
			GCurrentData.projectDirectoryPath + Name::Project_UserFiles       //new
		);
	}

}

void FD::ProjectWrite::save_layout() {
	using namespace Layout::Internal;
	using namespace Project::Internal;

	//already locked

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_Layout, std::ios::trunc);

		if (!ofs)
			throw std::runtime_error("Failed to open .layout file.");

		ofs << *LayoutData::mainFrameLeft << std::endl;
		ofs << *LayoutData::mainFrameRight << std::endl;
		ofs << *LayoutData::mainFrameTop << std::endl;
		ofs << *LayoutData::mainFrameBottom << std::endl;

		for (auto& x : LayoutData::history) {
			if (x.horizonal)
				ofs << "true" << std::endl;
			else
				ofs << "false" << std::endl;

			ofs << *x.pos << std::endl;

			//midpoint
			ofs << (*x.pos_side1 + ((*x.pos_side2 - *x.pos_side1) / 2.0f)) << std::endl;
		}

		ofs << Project::Internal::Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_Layout, //old
			GCurrentData.projectDirectoryPath + Name::Project_Layout       //new
		);
	}
}

void FD::ProjectWrite::backup() {
	using namespace Project::Internal;
	//assert(GCurrentData.projectFolderPath.back() == '/');

	//std::string path = Internal::Project::GCurrentData.projectFolderPath + "Backup/";
	//path += Internal::Project::getCurrentTime() += ".backup";

	//std::lock_guard<std::mutex> lock(Internal::Project::GMtx);

	////backupƒtƒHƒ‹ƒ_‚ªÁ‚³‚ê‚Ä‚¢‚½‚çì‚é
	//this->tryCreateBackupFolder();

	//this->write(path.c_str());
}

void FD::ProjectWrite::removeHistory(const std::string& fprojPath) {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(GMtx);

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	std::vector<HistoryInfo> temp(50);

	UIF16 index = std::numeric_limits<UIF16>::max();

	//til 50
	for (UIF16 i = 0; i < 50; i++) {
		//projectname path time
		std::getline(ifs, temp[i].projectName);
		std::getline(ifs, temp[i].projectFilePath);
		std::getline(ifs, temp[i].ymd_h);

		if (temp[i].projectFilePath == fprojPath) {
			index = i;
		}
	}

	if (index == std::numeric_limits<UIF16>::max())
		throw Exception::NotFoundHistory;

	temp.erase(temp.begin() + index);

	//rewrite
	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::HistoryTempPath, std::ios::trunc);

		if (!ofs)
			throw Exception::Unexpected;

		for (const auto& x : temp) {
			ofs << x.projectName << std::endl;
			ofs << x.projectFilePath << std::endl;
			ofs << x.ymd_h << std::endl;
		}
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::HistoryTempPath, //old
			Internal::Resource::RecentProjectHistoryFilePath           //new
		);
	}

}

void FD::ProjectWrite::writeProjectInfo(const std::string& path) {
	using namespace Project::Internal;

	std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::ProjectFileTempPath, std::ios::out | std::ios::binary);

	if (!ofs)
		throw Exception::Unexpected;

	//Since uint64_t is promised to be 64 bits or more, write it as 8 bytes fixed.
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
	ofs << GCurrentData.projectDirectoryPath << std::endl;

	ofs.close();

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::ProjectFileTempPath, //old
			path												           //new
		);
	}
}

void FD::ProjectWrite::tryCreateFluidumDirectory() {
	using namespace Project::Internal;

	constexpr const char* paths[] = {
		Name::FluidumHiddenDirectory,
		Name::ProjectDirectory,
		Name::GenomeDirectory,
		Name::BackupDirectory,
		Name::TempDirectory,
		Name::SrcDirectory
	};

	for (auto x : paths) {
		std::string path = GCurrentData.projectDirectoryPath + x;
		if (!std::filesystem::is_directory(path)) {
			if (!std::filesystem::create_directory(path))
				throw - 1;
		}
	}

	//hide
	std::string path = GCurrentData.projectDirectoryPath + paths[0];
	FU::File::hide(path);
}

void FD::ProjectWrite::tryCreateFluidumFiles() {
	using namespace Project::Internal;

	if (!std::filesystem::is_directory(GCurrentData.projectDirectoryPath + Name::FluidumHiddenDirectory))
		throw Exception::NotFoundProjectFiles;

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
		std::string path = GCurrentData.projectDirectoryPath + x;

		if (std::filesystem::exists(path))
			continue;

		std::ofstream ofs(path);
		if (!ofs)
			throw Exception::Unexpected;
	}

}

void FD::ProjectWrite::checkIsProjectFolderExist() const {
	using namespace Project::Internal;

	try {
		if (!std::filesystem::is_directory(GCurrentData.projectDirectoryPath))
			throw Exception::NotFoundProjectDirectory;
	}
	catch (...) {
		throw Exception::Unexpected;
	}
}

void FD::ProjectWrite::checkIsProjectFileExist() const {
	using namespace Project::Internal;

	try {
		if (!std::filesystem::exists(GCurrentData.projectFilePath))
			throw Exception::NotFoundProjectFile;
	}
	catch (...) {
		throw Exception::Unexpected;
	}
}

namespace FD::Project::Internal {
	[[nodiscard]] std::string getYMDH() {
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

void FD::ProjectWrite::updateHistory() {
	using namespace Project::Internal;

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	std::deque<HistoryInfo> temp(50);

	const std::string currentPath = GCurrentData.projectDirectoryPath + GCurrentData.projectName + ".fproj";
	UIF16 index = std::numeric_limits<UIF16>::max();
	bool isNewFile = true;//is new file

	//till 100
	for (UIF16 i = 0; i < 50; i++) {
		//projectname path time
		std::getline(ifs, temp[i].projectName);
		std::getline(ifs, temp[i].projectFilePath);
		std::getline(ifs, temp[i].ymd_h);

		if (temp[i].projectFilePath == currentPath) {
			index = i;
			isNewFile = false;
		}
	}

	//If there is a current file, bring it to the top.
	if (!isNewFile) {
		temp.emplace_front(temp.at(index));
		temp.erase(temp.begin() + index + 1);
	}
	//new
	else {
		HistoryInfo info;
		info.projectFilePath = currentPath;
		info.projectName = GCurrentData.projectName;
		info.ymd_h = getYMDH();
		temp.emplace_front(std::move(info));
	}


	//rewrite
	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::HistoryTempPath, std::ios::trunc);

		if (!ofs)
			throw Exception::Unexpected;

		for (const auto& x : temp) {
			ofs << x.projectName << std::endl;
			ofs << x.projectFilePath << std::endl;
			ofs << x.ymd_h << std::endl;
		}
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::rename(
			GCurrentData.projectDirectoryPath + Name::HistoryTempPath, //old
			Internal::Resource::RecentProjectHistoryFilePath           //new												           //new
		);
	}

}

void FD::ProjectWrite::readProjectInfo(std::ifstream& ifs) const {
	using namespace Project::Internal;

	std::string data{};

	//Fluidum
	std::getline(ifs, data);
	std::getline(ifs, data);
	if (data != "Fluidum")
		throw Exception::IllegalFile;

	//ProjectFilePath
	std::getline(ifs, data);
	if (data != "ProjectFilePath")
		throw Exception::IllegalFile;
	std::getline(ifs, data);
	GCurrentData.projectFilePath = data;

	//Time
	std::getline(ifs, data);
	if (data != "Time")
		throw Exception::IllegalFile;
	std::getline(ifs, data);

	//ProjectName
	std::getline(ifs, data);
	if (data != "ProjectName")
		throw Exception::IllegalFile;
	std::getline(ifs, data);
	GCurrentData.projectName = data;

	//ProjectFolderPath
	std::getline(ifs, data);
	if (data != "ProjectFolderPath")
		throw Exception::IllegalFile;
	std::getline(ifs, data);
	GCurrentData.projectDirectoryPath = data;

}

FD::Project::FileList::FileInfo FD::ProjectWrite::readFiles_element(std::ifstream& ifs) const {
	std::string data{};

	Project::FileList::FileInfo info;

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
	using Type = Project::Internal::FileList::Type;
	if (data == "D")
		info.type = Type::Directory;
	else if (data == "S")
		info.type = Type::Supported;
	else if (data == "U")
		info.type = Type::Unsupported;

	return info;
}

void FD::ProjectWrite::read_files_recursive(std::ifstream& ifs, Project::FileList::FileInfo* parent) const {
	std::string data{};

	parent->dir_internal.emplace_back(this->readFiles_element(ifs));

	//size
	std::getline(ifs, data);
	std::size_t size = std::stoull(data);
	for (std::size_t i = 0; i < size; i++) {
		this->read_files_recursive(ifs, &parent->dir_internal.back());
	}
}

void FD::ProjectWrite::read_fluidumFiles() const {
	using namespace Project::Internal;

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_FluidumFiles);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	//MainCodeFilePath
	std::getline(ifs, buf);

	FluidumFilesData::mainCodeFilePath = buf;
}

void FD::ProjectWrite::read_projectFiles() const {
	using namespace Project::Internal;

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_ProjectFiles);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	std::size_t counter = 0;
	while (true) {
		std::getline(ifs, buf);
		if (buf == Name::Delimiter)
			break;

		auto data = UserFilesData::userFiles.get();
		auto info = this->readFiles_element(ifs);
		data->emplace_back(std::move(info));
		this->read_files_recursive(ifs, &info);

		counter++;
		if (counter > 1000)
			throw Exception::BrokenFile;
	}
}

void FD::ProjectWrite::read_userFiles() const {
	using namespace Project::Internal;

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_UserFiles);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	std::size_t counter = 0;
	while (true) {
		std::getline(ifs, buf);
		if (buf == Name::Delimiter)
			break;

		auto data = UserFilesData::userFiles.get();
		auto info = this->readFiles_element(ifs);
		data->emplace_back(std::move(info));
		this->read_files_recursive(ifs, &info);

		counter++;
		if (counter > 5000)
			throw Exception::BrokenFile;
	}
}

void FD::ProjectWrite::read_layout() const {
	using namespace Project::Internal;
	using namespace Layout::Internal;

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_Layout);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

	LayoutData::history.clear();

	if (FU::File::empty(ifs))
		return;

	std::string buf{};

	//main frame
	try {
		std::getline(ifs, buf);
		LayoutData::mainFrameLeft = std::make_shared<float>(std::stof(buf));
		std::getline(ifs, buf);
		LayoutData::mainFrameRight = std::make_shared<float>(std::stof(buf));
		std::getline(ifs, buf);
		LayoutData::mainFrameTop = std::make_shared<float>(std::stof(buf));
		std::getline(ifs, buf);
		LayoutData::mainFrameBottom = std::make_shared<float>(std::stof(buf));
	}
	catch (const std::exception&) {
		throw Exception::BrokenFile;
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
			throw Exception::BrokenFile;

		try {
			if (his.horizonal) {
				std::getline(ifs, buf);
				his.readPos.y = std::stof(buf);
				std::getline(ifs, buf);
				his.readPos.x = std::stof(buf);
			}
			else {
				std::getline(ifs, buf);
				his.readPos.x = std::stof(buf);
				std::getline(ifs, buf);
				his.readPos.y = std::stof(buf);
			}
		}
		catch (const std::exception&) {
			throw Exception::BrokenFile;
		}


		LayoutData::history.emplace_back(his);


		counter++;
		if (counter > 200)
			throw Exception::BrokenFile;
	}
}

void FD::ProjectWrite::read_tab() const {
	using namespace Project::Internal;

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_Tab);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

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
			Coding::Internal::Data::displayFiles.emplace_back(data);

			counter++;
			if (counter > 1000)
				throw Exception::BrokenFile;
		}
	}

	//TabFilePathes
	{
		uint16_t counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == Name::Delimiter)
				break;
			Coding::Internal::Data::filePaths.emplace_back(data);

			counter++;
			if (counter > 1000)
				throw Exception::BrokenFile;
		}
	}
}

void FD::ProjectWrite::read_scene() const {
	using namespace Project::Internal;

	//already locked

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_Scene);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return;

	std::string data{};

	//SceneCodes
	{
		Size counter = 0;
		while (true) {
			std::getline(ifs, data);
			if (data == Name::Delimiter)
				break;

			const auto find = std::find_if(
				std::begin(::FS::Utils::Class::ClassCodesViewNames),
				std::end(::FS::Utils::Class::ClassCodesViewNames),
				[&](auto& x) {return x == data; }
			);
			if (find != std::end(::FS::Utils::Class::ClassCodesViewNames)) {
				const Size distance = static_cast<Size>(std::distance(std::begin(::FS::Utils::Class::ClassCodesViewNames), find));
				const auto code = ::FS::Utils::Class::ClassCodesView[distance];
				Scene::Internal::Data::codes.emplace_back(code);
			}

			counter++;
			if (counter > 1000)
				throw Exception::BrokenFile;
		}
	}

}

std::array<FD::ProjectRead::HistoryInfo, 50> FD::ProjectRead::loadProjectHistory() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(GMtx);

	std::ifstream ifs(Internal::Resource::RecentProjectHistoryFilePath);

	if (!ifs)
		throw Exception::Unexpected;

	std::array<HistoryInfo, 50> result{};

	//till 50
	for (Size i = 0; i < std::tuple_size_v<decltype(result)>; i++) {
		//projectname path time
		std::getline(ifs, result[i].projectName);
		std::getline(ifs, result[i].projectFilePath);
		std::getline(ifs, result[i].ymd_h);
	}

	return result;
}

std::vector<FU::Class::ClassCode::CodeType> FD::ProjectRead::loadSceneFile() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);

	std::ifstream ifs(Project::Internal::GCurrentData.projectDirectoryPath + Project::Internal::Name::Project_Scene);

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

std::string FD::ProjectRead::projectDirectoryPath() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectDirectoryPath;
}

std::string FD::ProjectRead::backupDirectoryPath() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectDirectoryPath + Project::Internal::Name::BackupDirectory;
}

std::string FD::ProjectRead::srcDirectoryPath() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectDirectoryPath + Project::Internal::Name::SrcDirectory;
}

std::string FD::ProjectRead::projectName() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.projectName;
}

bool FD::ProjectRead::isDefaultProject() const {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
	return Project::Internal::GCurrentData.isTempProject;
}
