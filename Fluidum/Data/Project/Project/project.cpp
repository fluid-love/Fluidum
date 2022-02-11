#include "project.h"
#include "../../Coding/tab.h"
#include "../../Scene/scene.h"
#include "../Files/files.h"
#include "../../Gui/layout.h"
#include "../../../Scene/Utils/Scene/classcodes.h"
#include "name.h"
#include "../../../Common/define.h"

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

}

namespace FD::Project::Internal {

	//helper
	[[nodiscard]] boost::json::value makeJsonValue(const std::string& filePath) {
		namespace json = boost::json;

		std::ifstream ifs(filePath);
		if (!ifs)
			throw ProjectWrite::Exception::NotFoundProjectFiles;
		if (FU::File::empty(ifs))
			throw ProjectWrite::Exception::FileEmpty;

		json::stream_parser p;
		json::error_code ec;

		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		p.write(str.c_str(), str.size(), ec);

		if (ec)
			throw ProjectWrite::Exception::BrokenFile;
		p.finish(ec);
		if (ec)
			throw ProjectWrite::Exception::BrokenFile;

		return p.release();
	}

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
		if (Project::Property::Internal::Data::save) {
			Project::Property::Internal::Data::save.store(false);
			std::lock_guard<std::mutex> lock(Project::Internal::GMtx);
			std::lock_guard<std::mutex> lockProperty(Project::Property::Internal::Data::mtx);
			this->save_projectProperty();
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

void FD::ProjectWrite::resetThread() {
	assert(this->joinable());
	this->saveThread = std::jthread{ &ProjectWrite::save_thread, this };
}

void FD::ProjectWrite::requestStop() noexcept {
	this->saveThread.request_stop();
}

bool FD::ProjectWrite::joinable() const noexcept {
	return this->saveThread.joinable();
}

namespace FD::Project::Internal {

	[[nodiscard]] std::string getCurrentZoneTime() {
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

	[[nodiscard]] std::string getCurrentTime() {
		const auto time1 = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time();
		const auto time2 = std::chrono::floor<std::chrono::days>(time1);
		std::chrono::year_month_day ymd(time2);

		std::chrono::hh_mm_ss time(std::chrono::floor< std::chrono::milliseconds>(time1 - time2));

		std::stringstream ss;
		ss << ymd.year() << '_' << ymd.month() << '_' << ymd.day() << '_'
			<< time.hours().count() << '_' << time.minutes().count() << '_' << time.seconds().count() << '_' << time.subseconds().count();
		return ss.str();
	}

}

//Constructor
FD::ProjectWrite::ProjectWrite(Internal::PassKey) noexcept {
	using namespace Project::Internal;

	try {
		//remove imgui.ini
		if (std::filesystem::exists("imgui.ini")) {
			const bool success = std::filesystem::remove("imgui.ini");
			if (!success)
				throw - 1;
		}
		
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

		this->writeProjectInfo(path);
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
		FU::File::tryPushSlash(GCurrentData.projectDirectoryPath);

		//not found
		if (!std::filesystem::is_directory(GCurrentData.projectDirectoryPath))
			throw Exception::NotFoundDirectory;

		GCurrentData.projectDirectoryPath += (GCurrentData.projectName + '/');

		{
			std::string path = GCurrentData.projectDirectoryPath;
			//directory
			if (std::filesystem::is_directory(path))
				throw Exception::AlreadyExist;

			//file
			path.pop_back();
			if (std::filesystem::exists(path))
				throw Exception::AlreadyExist;
		}
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
		this->writeProjectInfo(path);
		this->updateHistory();
	}
	catch (Exception) {
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
		FU::Exception::rethrow();
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

	//project property
	const auto temp_projectProperty_currentProjectType = Project::Property::Internal::Data::currentProjectType;
	std::string temp_projectPropertyLua_entryFilePath{};

	//files
	Project::Internal::FileList temp_files_projectFiles{};
	Project::Internal::FileList temp_files_userFiles{};

	//tab
	std::vector<std::string> temp_tab_filePaths{};
	std::vector<FD::Coding::DisplayInfo> temp_tab_displayInfo{};

	//scene
	std::vector<FU::Class::ClassCode::CodeType> temp_scene_codes{};

	//layout
	std::vector<Layout::Internal::History> temp_layout_history{};

	std::lock_guard<std::mutex> lock1(Project::Property::Internal::Data::mtx);
	std::lock_guard<std::mutex> lock3(ProjectFilesData::mtx);
	std::lock_guard<std::mutex> lock4(UserFilesData::mtx);
	std::unique_lock<std::mutex> lock5(Layout::Internal::LayoutData::mtx);
	std::lock_guard<std::mutex> lock6(Coding::Internal::Data::mtx);
	std::lock_guard<std::mutex> lock7(Scene::Internal::Data::mtx);

	try {//copy
		temp = GCurrentData;
		temp_projectPropertyLua_entryFilePath = Project::Property::Internal::LuaData::entryFilePath;
		temp_files_projectFiles = Project::Internal::ProjectFilesData::projectFiles;
		temp_files_userFiles = Project::Internal::UserFilesData::userFiles;
		temp_layout_history = Layout::Internal::LayoutData::history;
		temp_tab_displayInfo = Coding::Internal::Data::displayInfo;
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

	//thread
	this->requestStop(); //save old data.
	while (!this->joinable()) { ; }

	try {
		UI64 identifier = 0;
		ifs.read(reinterpret_cast<char*>(&identifier), 8);//identifier

		this->readProjectInfo(ifs);
		this->checkIsProjectFolderExist();

		//Project
		this->read_fluidumFiles();
		this->read_projectFiles();
		this->read_userFiles();

		this->read_layout();

		this->read_tab();
		this->read_scene();

		this->read_projectProperty();
		this->read_imguiini();

		this->updateHistory();

	}
	catch (...) {
		GCurrentData = std::move(temp);

		Project::Property::Internal::Data::currentProjectType = temp_projectProperty_currentProjectType;
		Project::Property::Internal::LuaData::entryFilePath = temp_projectPropertyLua_entryFilePath;

		Project::Internal::ProjectFilesData::projectFiles = std::move(temp_files_userFiles);
		Project::Internal::UserFilesData::userFiles = std::move(temp_files_projectFiles);

		Coding::Internal::Data::displayInfo = std::move(temp_tab_displayInfo);
		Coding::Internal::Data::filePaths = std::move(temp_tab_filePaths);
		Coding::Internal::Data::ProjectWrite::clearInternalData();

		Scene::Internal::Data::codes = std::move(temp_scene_codes);
		Layout::Internal::LayoutData::history = std::move(temp_layout_history);

		this->resetThread();
		FU::Exception::rethrow();
	}

	this->resetThread();

	GCurrentData.isTempProject = false;
}

void FD::ProjectWrite::saveAs(const std::string& newName, const std::string& dstProjectDirectoryPath) {
	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);

	std::string projectDirectoryPath = FU::File::consistentDirectory(dstProjectDirectoryPath);
	FU::File::tryPushSlash(projectDirectoryPath);
	const std::string fullPath = projectDirectoryPath + newName;

	//First, check if there is a folder with the same name in the destination folder.

	if (!std::filesystem::is_directory(projectDirectoryPath))
		throw Exception::NotFoundDirectory;
	if (std::filesystem::exists(fullPath))
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

	//thread
	this->requestStop(); //save old data.
	while (!this->joinable()) { ; }

	//copy
	try {
		const std::string srcPath = GCurrentData.projectDirectoryPath;
		const std::string oldProjPath = GCurrentData.projectFilePath;

		//set new directory path
		GCurrentData.projectDirectoryPath = fullPath + '/';
		GCurrentData.projectFilePath = GCurrentData.projectDirectoryPath + newName + ".fproj";
		GCurrentData.projectName = newName;

		std::filesystem::copy(
			srcPath, //from
			GCurrentData.projectDirectoryPath, //to
			std::filesystem::copy_options::recursive
		);

		//remove
		{
			const std::filesystem::path old = oldProjPath;
			const std::filesystem::path new_ = GCurrentData.projectFilePath;
			if (old.filename() != new_.filename())
				std::filesystem::remove(old);
		}

		this->writeProjectInfo(GCurrentData.projectFilePath);


		this->updateHistory();
	}
	catch (...) {
		std::filesystem::remove_all(fullPath);
		GCurrentData = temp;
		std::rethrow_exception(std::current_exception());
	}

	GCurrentData.isTempProject = false;
}

void FD::ProjectWrite::save_tab() {
	using namespace Project::Internal;
	namespace json = boost::json;

	//already locked

	std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_Tab, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .tab file.");

	json::object obj{};

	//display files
	{
		json::object& files = obj["DisplayFiles"].emplace_object();
		for (Size i = 0; const auto & x : Coding::Internal::Data::displayInfo) {
			i++;
			json::object& item = files[std::to_string(i)].emplace_object();
			item["FilePath"] = x.path;
			item["ZoomRatio"] = x.zoomRatio;

			//theme
			{
				using enum FD::Coding::DisplayInfo::Theme;
				if (x.theme == Default)
					item["Theme"] = "Default";
				else if (x.theme == Dark)
					item["Theme"] = "Dark";
				else if (x.theme == Light)
					item["Theme"] = "Light";
				else if (x.theme == Blue)
					item["Theme"] = "Blue";
				else {
					FluidumData_Log_Internal_InternalWarning();
				}
			}
		}
	}

	//tab files
	{
		json::object& files = obj["TabFiles"].emplace_object();
		for (Size i = 0; const auto & x : Coding::Internal::Data::filePaths) {
			i++;
			json::object& item = files[std::to_string(i)].emplace_object();
			item["FilePath"] = x;
		}
	}

	//This function may be called continuously. 
	//Copying file is a time-consuming operation.
	if (::FD::Scene::Internal::Data::save.load())
		return;

	{
		const std::string text = json::serialize(obj);
		ofs << text;
		ofs.close();
	}

	//success
	{
		//std::filesystem::error
		std::filesystem::copy(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_Tab, //from
			GCurrentData.projectDirectoryPath + Name::Project_Tab,      //new
			std::filesystem::copy_options::overwrite_existing
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

		bool copy = false;
		for (const auto& x : ::FD::Scene::Internal::Data::codes) {
			const auto find = std::find(std::begin(::FS::Utils::Class::ClassCodesView), std::end(::FS::Utils::Class::ClassCodesView), x);
			if (find != std::end(::FS::Utils::Class::ClassCodesView)) {
				const auto distance = std::distance(std::begin(::FS::Utils::Class::ClassCodesView), find);
				ofs << ::FS::Utils::Class::ClassCodesViewNames[distance] << std::endl;
				copy = true;
			}
		}

		//If we do not need to save the file, do not copy it.
		//std::filesystem_copy_file is slow.
		if (!copy)
			return;

		ofs << Project::Internal::Name::Delimiter << std::endl;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_Scene, //from
			GCurrentData.projectDirectoryPath + Name::Project_Scene,      //to
			std::filesystem::copy_options::overwrite_existing
		);
	}
}

void FD::ProjectWrite::save_files_recursive(boost::json::object& obj, const Project::FileList::FileInfo* info) {
	namespace json = boost::json;

	json::object& item = obj[info->path].emplace_object();
	{
		item["Name"] = info->name;
		item["Open"] = info->open;

		//type
		const char* type;
		if (info->type == FD::Project::FileList::Type::Directory)
			type = "Dir";
		else if (info->type == FD::Project::FileList::Type::Supported)
			type = "Sup";
		else if (info->type == FD::Project::FileList::Type::Unsupported)
			type = "Unsup";
		else {
			FluidumData_Log_Internal_InternalError();
			throw Exception::Unexpected;
		}
		item["Type"] = type;
	}

	for (Size i = 0, size = info->dir_internal.size(); i < size; i++) {
		save_files_recursive(item, &info->dir_internal[i]);
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

		boost::json::object obj{};

		for (auto& x : *data) {
			this->save_files_recursive(obj, &x);
		}

		const std::string text = boost::json::serialize(obj);

		ofs << text;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_ProjectFiles, //from
			GCurrentData.projectDirectoryPath + Name::Project_ProjectFiles,      //to
			std::filesystem::copy_options::overwrite_existing
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

		boost::json::object obj{};

		for (auto& x : *data) {
			this->save_files_recursive(obj, &x);
		}

		const std::string text = boost::json::serialize(obj);

		ofs << text;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_UserFiles, //from
			GCurrentData.projectDirectoryPath + Name::Project_UserFiles,      //to
			std::filesystem::copy_options::overwrite_existing
		);
	}

}

void FD::ProjectWrite::save_layout() {
	using namespace Layout::Internal;
	using namespace Project::Internal;

	namespace json = boost::json;

	//already locked

	std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_Layout, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .layout file.");

	//Save the file as a ratio, since the screen size may be different.
	auto toRatio = [&](const bool horizonal, const float pos) -> float {
		if (horizonal) {
			return (pos - *LayoutData::mainFrameTop) / (*LayoutData::mainFrameBottom - *LayoutData::mainFrameTop);
		}
		else {
			return (pos - *LayoutData::mainFrameLeft) / (*LayoutData::mainFrameRight - *LayoutData::mainFrameLeft);
		}
	};

	json::object obj{};

	for (Size i = 0; auto & x : LayoutData::history) {
		i++;

		json::object& item = obj[std::to_string(i)].emplace_object();
		item["Horizonal"] = x.horizonal;
		item["Pos"] = toRatio(x.horizonal, *x.pos);
		item["MidPos"] = toRatio(!x.horizonal, (*x.pos_side1 + ((*x.pos_side2 - *x.pos_side1) / 2.0f)));
	}

	{
		const std::string text = json::serialize(obj);
		ofs << text;
		ofs.close();
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_Layout, //from
			GCurrentData.projectDirectoryPath + Name::Project_Layout,      //to
			std::filesystem::copy_options::overwrite_existing
		);
	}
}

void FD::ProjectWrite::save_projectProperty() {
	using namespace Project::Internal;

	//already locked

	std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_ProjectProperty, std::ios::trunc);

	if (!ofs)
		throw std::runtime_error("Failed to open .projectproperty file.");

	namespace json = boost::json;

	json::object obj{};

	//common
	{
		json::object& common = obj["Common"].emplace_object();
		{
			using enum FD::Project::Property::ProjectType;
			const char* type;
			if (FD::Project::Property::Internal::Data::currentProjectType == None)
				type = "None";
			else if (FD::Project::Property::Internal::Data::currentProjectType == Lua)
				type = "Lua";
			else if (FD::Project::Property::Internal::Data::currentProjectType == Python)
				type = "Python";
			else if (FD::Project::Property::Internal::Data::currentProjectType == Cpp)
				type = "C++";
			else {
				FluidumData_Log_Internal_InternalWarning();
				type = "None";
			}
			common["ProjectType"] = type;
		}
	}

	//Lua
	{
		json::object& lua = obj["Lua"].emplace_object();
		{
			lua["EntryFilePath"] = FD::Project::Property::Internal::LuaData::entryFilePath;
		}
	}

	//write
	{
		const std::string text = json::serialize(obj);
		ofs << text;
		ofs.close();
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_ProjectProperty, //from
			GCurrentData.projectDirectoryPath + Name::Project_ProjectProperty,      //to
			std::filesystem::copy_options::overwrite_existing
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
	namespace json = boost::json;

	std::lock_guard<std::mutex> lock(GMtx);

	std::array<FD::ProjectRead::HistoryInfo, FD::Project::Limits::HistoryLogMax> result{};

	json::value val{};
	try {
		val = makeJsonValue(Internal::Resource::RecentProjectHistoryFilePath);
	}
	catch (const Exception) {
		FU::Exception::rethrow();
	}

	if (!val.is_object())
		throw Exception::BrokenFile;

	json::object& obj = val.get_object();

	const auto find = std::find_if(
		obj.cbegin(),
		obj.cend(),
		[&](auto& x) {
			return x.value().at("Path").get_string() == fprojPath;
		}
	);

	const bool notFound = (find == obj.cend());

	if (notFound)
		throw Exception::NotFoundHistory;

	//remove
	obj.erase(find);

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::HistoryTempPath, std::ios::trunc);
		const std::string text = json::serialize(obj);
		ofs << text;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::HistoryTempPath, //from
			Internal::Resource::RecentProjectHistoryFilePath,          //to
			std::filesystem::copy_options::overwrite_existing
		);
	}

}

void FD::ProjectWrite::clearHistory() {

	try {
		std::ofstream ofs(Internal::Resource::RecentProjectHistoryFilePath, std::ios::trunc);

		if (!ofs)
			throw - 1;
	}
	catch (...) {
		throw Exception::Unexpected;
	}

}

void FD::ProjectWrite::saveImGuiIniFile() {
	using namespace Project::Internal;
	try {
		const char* const text = ImGui::SaveIniSettingsToMemory();
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::Project_Temp_ImGuiIni);
		if (!ofs)
			throw Exception::Unexpected;
		ofs << text;
		ofs.close();

		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::Project_Temp_ImGuiIni, //from
			GCurrentData.projectDirectoryPath + Name::Project_ImGuiIni,      //to
			std::filesystem::copy_options::overwrite_existing
		);
	}
	catch (...) {
		Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to copy imgui init file.");
		FU::Exception::rethrow();
	}

}

void FD::ProjectWrite::writeProjectInfo(const std::string& path) {
	using namespace Project::Internal;
	namespace json = boost::json;

	json::object obj{};

	obj["Version"] = Fluidum::Version::constChar();
	obj["Created"] = getCurrentZoneTime();
	obj["ProjectFilePath"] = GCurrentData.projectFilePath;
	obj["ProjectName"] = GCurrentData.projectName;
	obj["ProjectFolderPath"] = GCurrentData.projectDirectoryPath;

	{
		std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::ProjectFileTempPath, std::ios::out | std::ios::binary | std::ios::trunc);

		if (!ofs)
			throw Exception::Unexpected;

		const std::string text = json::serialize(obj);
		ofs << text;
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy(
			GCurrentData.projectDirectoryPath + Name::ProjectFileTempPath, //from
			path,												           //to
			std::filesystem::copy_options::overwrite_existing
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
		Name::Project_ProjectFiles,
		Name::Project_UserFiles,
		Name::Project_Tab,
		Name::Project_Scene,
		Name::Project_Layout,
		Name::Project_ProjectProperty,
		Name::Project_ImGuiIni,
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
	namespace json = boost::json;

	//already locked

	const std::string currentProjectFilePath = GCurrentData.projectFilePath + GCurrentData.projectName + ".fproj";

	bool empty = false;

	json::value val{};
	try {
		val = makeJsonValue(Internal::Resource::RecentProjectHistoryFilePath);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			empty = true;
		else
			FU::Exception::rethrow();
	}

	std::ofstream ofs(GCurrentData.projectDirectoryPath + Name::HistoryTempPath, std::ios::trunc);

	if (!ofs)
		throw Exception::Unexpected;

	if (!empty && !val.is_object())
		throw Exception::BrokenFile;

	bool newProject = true;
	std::vector<HistoryInfo>::const_iterator oldProjectItr{};

	std::vector<HistoryInfo> temp(Project::Limits::HistoryLogMax);

	if (!empty) {
		json::object& obj = val.get_object();
		Size index = 0;
		for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++, index++) {
			if (index == Project::Limits::HistoryLogMax)
				break;

			HistoryInfo info{};
			const json::value val = itr->value();
			info.projectFilePath = val.at("Path").get_string();
			info.projectName = val.at("Name").get_string();
			info.ymd_h = val.at("Date").get_string();

			if (!newProject && info.projectFilePath == currentProjectFilePath) {
				newProject = false;
				oldProjectItr = temp.begin() + index;
			}

			temp[index] = std::move(info);
		}
	}

	json::object obj{};

	if (newProject) {
		json::object item{};
		item["Path"] = currentProjectFilePath;
		item["Name"] = GCurrentData.projectName;
		item["Date"] = getYMDH();

		obj["1"].emplace_object() = std::move(item);
	}
	else { //to top
		HistoryInfo info = *oldProjectItr;
		info.ymd_h = getYMDH();//update
		temp.erase(oldProjectItr);
		temp.insert(temp.begin(), std::move(info));
	}

	//rewrite
	{
		Size index = newProject ? 2 : 1;
		for (Size i = 0, size = temp.size(); i < size; i++, index++) {
			if (i == Project::Limits::HistoryLogMax)
				break;
			if (temp[i].projectFilePath.empty())
				break;

			json::object item{};
			item["Path"] = temp[i].projectFilePath;
			item["Name"] = temp[i].projectName;
			item["Date"] = temp[i].ymd_h;

			obj[std::to_string(index)].emplace_object() = std::move(item);
		}
	}

	{
		const std::string text = json::serialize(obj);
		ofs << text;
		ofs.close();
	}

	//success
	{
		//std::filesystem::filesystem_error
		std::filesystem::copy_file(
			GCurrentData.projectDirectoryPath + Name::HistoryTempPath, //from
			Internal::Resource::RecentProjectHistoryFilePath,          //to	
			std::filesystem::copy_options::overwrite_existing
		);
	}

}

void FD::ProjectWrite::readProjectInfo(std::ifstream& ifs) const {
	using namespace Project::Internal;
	namespace json = boost::json;

	json::value val{};
	try {
		val = makeJsonValue(GCurrentData.projectDirectoryPath + Name::Project_Layout);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			return;
		FU::Exception::rethrow();
	}

	GCurrentData.projectFilePath = val.at("ProjectFilePath").get_string();
	GCurrentData.projectName = val.at("ProjectName").get_string();
	GCurrentData.projectDirectoryPath = val.at("ProjectFolderPath").get_string();

}

FD::Project::FileList::FileInfo FD::ProjectWrite::readFiles_element(const boost::json::value& val) const {
	FD::Project::FileList::FileInfo info{};

	info.name = val.at("Name").get_string();
	info.open = val.at("Open").get_bool();

	const std::string type = val.at("Type").get_string().c_str();

	using enum Project::Internal::FileList::Type;
	if (type == "Dir")
		info.type = Directory;
	else if (type == "Sup")
		info.type = Supported;
	else if (type == "Unsup")
		info.type = Unsupported;
	else
		throw Exception::BrokenFile;

	return info;
}

void FD::ProjectWrite::read_files_recursive(const bool top, std::vector< FD::Project::Internal::FileList::Ref>* data, const boost::json::value& val, Project::FileList::FileInfo* parent) const {
	namespace json = boost::json;

	if (val.kind() != json::kind::object) {
		return;
	}

	const json::object& obj = val.get_object();

	if (top) {
		for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++) {
			if (!itr->value().is_object())
				continue;

			FD::Project::FileList::FileInfo info = this->readFiles_element(itr->value());
			info.path = itr->key();
			data->emplace_back(std::move(info));
			this->read_files_recursive(false, nullptr, itr->value(), &data->back());
		}
	}
	else {
		for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++) {
			if (!itr->value().is_object())
				continue;

			FD::Project::FileList::FileInfo info = this->readFiles_element(itr->value());
			info.path = itr->key();
			parent->dir_internal.emplace_back(std::move(info));
			this->read_files_recursive(false, nullptr, itr->value(), &parent->dir_internal.back());
		}
	}


}

void FD::ProjectWrite::read_fluidumFiles() const {
	//using namespace Project::Internal;

	//std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_FluidumFiles);
	//if (!ifs)
	//	throw Exception::NotFoundProjectFiles;

	//if (FU::File::empty(ifs))
	//	return;

	//std::string buf{};

	////MainCodeFilePath
	//std::getline(ifs, buf);

	//FluidumFilesData::mainCodeFilePath = buf;
}

void FD::ProjectWrite::read_projectFiles() const {
	using namespace Project::Internal;
	namespace json = boost::json;

	json::value val{};
	try {
		val = makeJsonValue(GCurrentData.projectDirectoryPath + Name::Project_ProjectFiles);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			return;
		FU::Exception::rethrow();
	}

	auto data = ProjectFilesData::projectFiles.get();
	data->clear();

	if (!val.is_object())
		throw Exception::Unexpected;

	this->read_files_recursive(true, data, val, nullptr);
}

void FD::ProjectWrite::read_userFiles() const {
	using namespace Project::Internal;
	namespace json = boost::json;

	json::value val{};
	try {
		val = makeJsonValue(GCurrentData.projectDirectoryPath + Name::Project_UserFiles);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			return;
		FU::Exception::rethrow();
	}

	auto data = UserFilesData::userFiles.get();
	data->clear();

	const json::object& obj = val.get_object();

	this->read_files_recursive(true, data, val, nullptr);
}

void FD::ProjectWrite::read_layout() const {
	using namespace Project::Internal;
	using namespace Layout::Internal;
	namespace json = boost::json;

	json::value val{};
	try {
		val = makeJsonValue(GCurrentData.projectDirectoryPath + Name::Project_Layout);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			return;
		FU::Exception::rethrow();
	}

	LayoutData::history.clear();

	if (!val.is_object())
		throw Exception::BrokenFile;

	const json::object& obj = val.get_object();

	for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++) {
		Layout::Internal::History his{};

		his.horizonal = itr->value().at("Horizonal").get_bool();
		const float pos1 = static_cast<float>(itr->value().at("Pos").get_double());
		const float pos2 = static_cast<float>(itr->value().at("MidPos").get_double());

		if (his.horizonal) {
			his.readPosRatio.y = pos1;
			his.readPosRatio.x = pos2;
		}
		else {
			his.readPosRatio.x = pos1;
			his.readPosRatio.y = pos2;
		}

		LayoutData::history.emplace_back(his);
	}

}

void FD::ProjectWrite::read_tab() const {
	using namespace Project::Internal;
	namespace json = boost::json;

	json::value val{};
	try {
		val = makeJsonValue(GCurrentData.projectDirectoryPath + Name::Project_Tab);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			return;
		FU::Exception::rethrow();
	}

	Coding::Internal::Data::displayInfo.clear();

	//display files
	{
		const json::value files = val.at("DisplayFiles");
		if (!files.is_object())
			throw Exception::BrokenFile;
		const json::object& obj = files.get_object();
		for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++) {
			const json::object& elm = itr->value().get_object();
			Coding::DisplayInfo info{};
			info.path = elm.at("FilePath").get_string();

			//zoom
			info.zoomRatio = static_cast<float>(elm.at("ZoomRatio").get_double());
			if (info.zoomRatio * 100.0f < Coding::TextEditor::Limits::ZoomMin)
				info.zoomRatio = Coding::TextEditor::Limits::ZoomMin / 100.0f;
			else if (info.zoomRatio * 100.0f > Coding::TextEditor::Limits::ZoomMax)
				info.zoomRatio = Coding::TextEditor::Limits::ZoomMax / 100.0f;

			//theme
			{
				using enum FD::Coding::DisplayInfo::Theme;
				const std::string theme = elm.at("Theme").get_string().c_str();
				if (theme == "Default")
					info.theme = Default;
				else if (theme == "Dark")
					info.theme = Dark;
				else if (theme == "Light")
					info.theme = Light;
				else if (theme == "Blue")
					info.theme = Blue;
				else {
					FluidumData_Log_Internal_InternalWarning();
					info.theme = Default;
				}
			}

			//Not exists. The file has been deleted or moved.
			if (!std::filesystem::exists(info.path))
				continue;

			Coding::Internal::Data::displayInfo.emplace_back(std::move(info));
		}
	}

	//tab files
	{
		const json::value files = val.at("TabFiles");
		if (!files.is_object())
			throw Exception::BrokenFile;
		const json::object& obj = files.get_object();
		for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++) {
			const json::object& elm = itr->value().get_object();
			std::string path = elm.at("FilePath").get_string().c_str();
			Coding::Internal::Data::filePaths.emplace_back(std::move(path));
		}
	}

	//set internal data.
	Coding::Internal::Data::ProjectWrite::initializeInternalData();

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

void FD::ProjectWrite::read_projectProperty() {
	using namespace Project::Internal;
	namespace json = boost::json;

	//already locked

	json::value val{};
	try {
		val = makeJsonValue(GCurrentData.projectDirectoryPath + Name::Project_ProjectProperty);
	}
	catch (const Exception val) {
		if (val == Exception::FileEmpty)
			return;
		FU::Exception::rethrow();
	}

	std::string data{};

	//common
	{
		const json::value common = val.at("Common");
		{
			const json::value projectType = common.at("ProjectType");
			if (!projectType.is_string())
				Exception::BrokenFile;

			using enum FD::Project::Property::ProjectType;

			const std::string type = projectType.get_string().c_str();
			if (type == "None")
				FD::Project::Property::Internal::Data::currentProjectType = None;
			else if (type == "Lua")
				FD::Project::Property::Internal::Data::currentProjectType = Lua;
			else if (type == "Python")
				FD::Project::Property::Internal::Data::currentProjectType = Python;
			else if (type == "C++")
				FD::Project::Property::Internal::Data::currentProjectType = Cpp;
			else {
				Internal::GMessenger.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Invalid pattern string({}).", type);
				throw Exception::BrokenFile;
			}
		}
	}

	//Lua
	{
		const json::value common = val.at("Lua");
		{
			const json::value entryFilePath = common.at("EntryFilePath");
			if (!entryFilePath.is_string())
				Exception::BrokenFile;

			using enum FD::Project::Property::ProjectType;

			std::string path = json::serialize(entryFilePath.get_string());
			FD::Project::Property::Internal::LuaData::entryFilePath = std::move(path);
		}
	}

}

void FD::ProjectWrite::read_imguiini() {
	using namespace Project::Internal;

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_ImGuiIni);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;
	if (FU::File::empty(ifs)) {
		ImGui::LoadIniSettingsFromMemory("");
		return;
	}

	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	ImGui::LoadIniSettingsFromMemory(str.c_str());
}

std::array<FD::ProjectRead::HistoryInfo, FD::Project::Limits::HistoryLogMax> FD::ProjectRead::loadProjectHistory() const {
	using namespace Project::Internal;
	namespace json = boost::json;

	std::lock_guard<std::mutex> lock(GMtx);

	std::array<FD::ProjectRead::HistoryInfo, FD::Project::Limits::HistoryLogMax> result{};

	json::value val{};
	try {
		val = makeJsonValue(Internal::Resource::RecentProjectHistoryFilePath);
	}
	catch (const Exception v) {
		if (v == Exception::FileEmpty)
			return{};
		else
			FU::Exception::rethrow();
	}

	if (!val.is_object())
		throw Exception::BrokenFile;

	const json::object& obj = val.get_object();

	Size index = 0;
	for (auto itr = obj.cbegin(), end = obj.cend(); itr != end; itr++, index++) {
		if (index == Project::Limits::HistoryLogMax)
			break;

		HistoryInfo info{};
		const json::value val = itr->value();
		info.projectFilePath = val.at("Path").get_string();
		info.projectName = val.at("Name").get_string();
		info.ymd_h = val.at("Date").get_string();

		result[index] = std::move(info);
	}

	return result;
}

std::vector<FU::Class::ClassCode::CodeType> FD::ProjectRead::loadSceneFile() const {
	using namespace Project::Internal;

	std::lock_guard<std::mutex> lock(Project::Internal::GMtx);

	std::ifstream ifs(GCurrentData.projectDirectoryPath + Name::Project_Scene);
	if (!ifs)
		throw Exception::NotFoundProjectFiles;

	if (FU::File::empty(ifs))
		return {};

	std::string data{};

	std::vector<FU::Class::ClassCode::CodeType> result{};

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
				result.emplace_back(code);
			}

			counter++;
			if (counter > 1000)
				throw Exception::BrokenFile;
		}
	}

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
