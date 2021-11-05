#include "files.h"
#include "../../Limits/limits.h"

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

void FD::ProjectFilesWrite::save() const {
	using namespace Project::Internal;
	ProjectFilesData::save.store(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::UserFilesWrite::eraseFile(const std::string& path) {
	using namespace Project::Internal;
	UserFilesData::userFiles.erase(path);
}

bool FD::UserFilesWrite::tryChangeName(const std::string& path, const std::string& newName) {
	using namespace Project::Internal;

	const bool sameName = UserFilesData::userFiles.sameName(path, newName);

	if (!sameName) {
		std::string newPath = path;
		UserFilesData::userFiles.changePathAndName(path, newName);
		return true;
	}

	return false;
}

std::string FD::UserFilesWrite::makeTempName() {
	using namespace Project::Internal;

	for (uint16_t i = 1; i < Project::Limits::UserFiles::DirectorySizeMax; i++) {
		std::string ret = "NewVFolder" + std::to_string(i);
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
	return std::unique_lock<std::mutex>(UserFilesData::mtx);
}

void FD::UserFilesWrite::save() const {
	using namespace Project::Internal;
	UserFilesData::save.store(true);
}


