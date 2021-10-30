#include "project_files.h"

void FD::ProjectFilesWrite::setMainCodePath(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);

	{
		auto itr = this->findCodeFile(path);
		if (itr == FileData::codeFilePathes.end())
			FileData::codeFilePathes.emplace_back(path);
	}

	FileData::mainCodeFilePath = path;
}

void FD::ProjectFilesWrite::addIncludeCodeFilePath(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);

	auto itr = this->findCodeFile(path);
	if (itr == FileData::codeFilePathes.end())
		FileData::codeFilePathes.emplace_back(path);

	FileData::includeCodeFilePathes.emplace_back(path);
}

void FD::ProjectFilesWrite::addCodeFilePath(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);
	FileData::codeFilePathes.emplace_back(path);
}

void FD::ProjectFilesWrite::unsetMainCodePath() const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);
	FileData::mainCodeFilePath.clear();
}

void FD::ProjectFilesWrite::eraseIncludeCodeFilePath(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);

	auto itr = this->findIncludeCodeFile(path);
	if (itr == FileData::includeCodeFilePathes.end())
		throw std::runtime_error("Failed to find.");

	FileData::includeCodeFilePathes.erase(itr);
}

void FD::ProjectFilesWrite::eraseCodeFilePath(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);
	
	{
	auto itr = this->findIncludeCodeFile(path);
	if (itr != FileData::includeCodeFilePathes.end())
		FileData::includeCodeFilePathes.erase(itr);
	}

	auto itr = this->findCodeFile(path);
	assert(itr != FileData::codeFilePathes.end());

	FileData::codeFilePathes.erase(itr);
}

std::vector<std::string>::iterator FD::ProjectFilesWrite::findCodeFile(const std::string& path) const {
	using namespace Project::Internal;
	return std::find(FileData::codeFilePathes.begin(), FileData::codeFilePathes.end(), path);
}

std::vector<std::string>::iterator FD::ProjectFilesWrite::findIncludeCodeFile(const std::string& path) const {
	using namespace Project::Internal;
	return std::find(FileData::includeCodeFilePathes.begin(), FileData::includeCodeFilePathes.end(), path);
}

void FD::ProjectFilesWrite::save() const {
	using namespace Project::Internal;
	FileData::save.store(true);
}

std::vector<std::string> FD::ProjectFilesRead::includeCodeFilePathes() const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);
	return FileData::includeCodeFilePathes;
}

std::string FD::ProjectFilesRead::mainCodeFilePath() const{
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);
	return FileData::mainCodeFilePath;
}

bool FD::ProjectFilesRead::isMainCodeFileExist() const{
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);
	return !FileData::mainCodeFilePath.empty();
}

FD::Project::CodeType FD::ProjectFilesRead::getCurrentMainCodeType() const{
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(FileData::mtx);

	if (FileData::mainCodeFilePath.empty())
		return Project::CodeType::Empty;

	std::filesystem::path path = FileData::mainCodeFilePath;
	std::string extension = path.extension().string();
	
	if (extension == ".py")
		return Project::CodeType::Python;
	else if (extension == ".lua")
		return Project::CodeType::Lua;
	else if (extension == ".as")
		return Project::CodeType::AngelScript;

	return Project::CodeType::Error;
}