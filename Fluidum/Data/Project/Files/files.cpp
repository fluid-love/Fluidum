#include "files.h"
#include "../../Limits/limits.h"
#include "../Project/name.h"

void FD::LuaFilesWrite::closeAll() {
	using namespace Project::Internal;
	LibraryFilesData::luaLibraries.forEach([](Project::List::FileInfo& info) {info.open = false; });
}

void FD::LuaFilesWrite::save() const {
	using namespace Project::Internal;
	LibraryFilesData::save.store(true);
}

std::vector<FD::Project::List::FileInfo>* FD::LuaFilesWrite::fileList() {
	using namespace Project::Internal;
	return LibraryFilesData::luaLibraries.get();
}

std::unique_lock<std::mutex> FD::LuaFilesWrite::getLock() {
	using namespace Project::Internal;
	FluidumUtils_Debug_BeginDisableAllWarning //VS C26115
		return std::unique_lock<std::mutex>(LibraryFilesData::mtx);
	FluidumUtils_Debug_EndDisableAllWarning
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::FluidumFilesWrite::setMainCodePath(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);

	FluidumFilesData::mainCodeFilePath = path;
}

void FD::FluidumFilesWrite::unsetMainCodePath() const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);
	FluidumFilesData::mainCodeFilePath.clear();
}

std::string FD::FluidumFilesRead::mainCodeFilePath() const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);
	return FluidumFilesData::mainCodeFilePath;
}

bool FD::FluidumFilesRead::isMainCodeFileExist() const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);
	return !FluidumFilesData::mainCodeFilePath.empty();
}

FD::Project::CodeType FD::FluidumFilesRead::getCurrentMainCodeType() const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FluidumFilesData::mtx);

	if (FluidumFilesData::mainCodeFilePath.empty())
		return Project::CodeType::Empty;

	std::filesystem::path path = FluidumFilesData::mainCodeFilePath;
	std::string extension = path.extension().string();

	if (extension == ".py")
		return Project::CodeType::Python;
	else if (extension == ".lua")
		return Project::CodeType::Lua;
	else if (extension == ".as")
		return Project::CodeType::AngelScript;

	return Project::CodeType::Error;
}

void FD::FluidumFilesWrite::save() const {
	using namespace Project::Internal;
	FluidumFilesData::save.store(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::ProjectFilesWrite::eraseFile(const std::string& path) {
	using namespace Project::Internal;
	ProjectFilesData::projectFiles.erase(path);
}

void FD::ProjectFilesWrite::changeName(const std::string& path, const std::string& newName) {
	using namespace Project::Internal;
	ProjectFilesData::projectFiles.changePathAndName(path, newName);
}

void FD::ProjectFilesWrite::sync(const std::string& top) {
	using namespace Project::Internal;

	ProjectFilesData::projectFiles.sync();

	auto* data = ProjectFilesData::projectFiles.get();

	using Info = Project::List::FileInfo;

	std::function<void(const std::string&)> func = [&](const std::string& parent) {

		for (const auto& entry : std::filesystem::directory_iterator(parent)) {

			if (entry.is_directory()) {
				if (entry.path().string() == Project::Internal::Name::FluidumHiddenFolder)
					continue;
				Project::List::DirectoryInfo dir{};
				Info* elm = ProjectFilesData::projectFiles.add(parent, entry.path().string(), dir);
				func(elm->path);
			}
			else {
				Project::List::CodeInfo code{};
				Info* elm = ProjectFilesData::projectFiles.add(parent, entry.path().string(), code);
			}

		}

	};

	for (const auto& entry : std::filesystem::directory_iterator(top)) {
		if (entry.is_directory()) {
			Project::List::DirectoryInfo dir{};
			Info* elm = ProjectFilesData::projectFiles.add({}, entry.path().string(), dir);
			func(elm->path);
		}
		else {
			Project::List::CodeInfo code{};
			Info* elm = ProjectFilesData::projectFiles.add({}, entry.path().string(), code);
		}
	}
}

void FD::ProjectFilesWrite::save() const {
	using namespace Project::Internal;
	ProjectFilesData::save.store(true);
}

std::vector<std::string> FD::ProjectFilesWrite::findOpenPaths() const {
	using namespace Project::Internal;
	std::vector<Project::List::FileInfo>* data = ProjectFilesData::projectFiles.get();

	using Info = Project::List::FileInfo;

	std::vector<std::string> openPaths{};
	std::function<void(const Info* parent)> getOpenPaths = [&](const Info* parent) {
		for (auto& x : parent->dir_internal) {
			if (x.open)
				openPaths.emplace_back(x.path);
			if (!x.dir_internal.empty())
				getOpenPaths(&x);
		}
	};
	for (auto& x : *data) {
		getOpenPaths(&x);
	}
	return openPaths;
}

void FD::ProjectFilesWrite::closeAll() {
	using namespace Project::Internal;
	ProjectFilesData::projectFiles.forEach([](Project::List::FileInfo& info) {info.open = false; });
}

std::vector<FD::Project::List::FileInfo>* FD::ProjectFilesWrite::fileList() {
	using namespace Project::Internal;
	return ProjectFilesData::projectFiles.get();
}

std::unique_lock<std::mutex> FD::ProjectFilesWrite::getLock() {
	using namespace Project::Internal;
	FluidumUtils_Debug_BeginDisableAllWarning //VS C26115
		return std::unique_lock<std::mutex>(ProjectFilesData::mtx);
	FluidumUtils_Debug_EndDisableAllWarning
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::UserFilesWrite::eraseFile(const std::string& path) {
	using namespace Project::Internal;
	UserFilesData::userFiles.erase(path);
}

bool FD::UserFilesWrite::tryChangeName(const std::string& path, const std::string& newName) {
	using namespace Project::Internal;

	const bool sameName = UserFilesData::userFiles.sameName(path, newName);

	if (sameName) {
		return false;
	}

	std::string newPath = path;
	UserFilesData::userFiles.changePathAndName(path, newName);
	return true;
}

std::string FD::UserFilesWrite::makeTempName() {
	using namespace Project::Internal;

	for (uint16_t i = 1; i < Project::Limits::UserFiles::DirectorySizeMax; i++) {
		std::string ret = "NewVirtualFolder" + std::to_string(i);
		if (!UserFilesData::userFiles.samePath(ret))
			return ret;
	}

	return {};
}

std::vector<FD::Project::List::FileInfo>* FD::UserFilesWrite::fileList() {
	using namespace Project::Internal;
	return UserFilesData::userFiles.get();
}

std::unique_lock<std::mutex> FD::UserFilesWrite::getLock() {
	using namespace Project::Internal;
	FluidumUtils_Debug_BeginDisableAllWarning //VS C26115
		return std::unique_lock<std::mutex>(UserFilesData::mtx);
	FluidumUtils_Debug_EndDisableAllWarning
}

void FD::UserFilesWrite::save() const {
	using namespace Project::Internal;
	UserFilesData::save.store(true);
}

void FD::UserFilesWrite::closeAll() {
	using namespace Project::Internal;
	UserFilesData::userFiles.forEach([](Project::List::FileInfo& info) {info.open = false; });
}

void FD::UserFilesWrite::sync() {
	using namespace Project::Internal;
	UserFilesData::userFiles.sync();
}












