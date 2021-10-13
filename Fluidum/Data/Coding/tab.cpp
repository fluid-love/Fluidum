#include "tab.h"

namespace FD::Internal::Coding {
	constexpr inline uint16_t LimitMaxFileSize = 1000;
	bool Update = false;
	bool DisplayFileChanged = false;

	struct FileInfo final {
		std::string code{};
	};

	std::map<std::string, FileInfo> GData;
}

using namespace ::FD::Internal::Coding;

void FD::Coding::TabWrite::addFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	if (TabData::filePathes.size() >= LimitMaxFileSize)
		throw Exception::LimitFileSizeMax;

	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr != TabData::filePathes.end())
		throw Exception::AlreadyExist;

	std::ifstream ifs(path);
	if (!ifs)
		throw Exception::NotFound;

	TabData::filePathes.emplace_back(path);

	this->update();
}

void FD::Coding::TabWrite::eraseFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;
	TabData::filePathes.erase(itr);

	this->update();
}

void FD::Coding::TabWrite::setDisplayFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;
	TabData::displayFile = *itr;

	DisplayFileChanged = true;
	this->update();
}

void FD::Coding::TabWrite::temp(const std::string& path, std::string&& code) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	FileInfo info{
		.code = std::move(code)
	};
	GData.insert({ path, std::move(info) });
}

std::string FD::Coding::TabWrite::getTemp(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	std::string code = std::move(GData.at(path).code);
	GData.erase(path);

	return code;
}

void FD::Coding::TabWrite::update() const {
	Update = true;
}

void FD::Coding::TabWrite::save() const {
	TabData::save.store(true);
}

bool FD::Coding::TabRead::update() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	bool result = Internal::Coding::Update;
	if (result) {
		Internal::Coding::Update = false;
	}
	return result;
}

std::vector<std::string> FD::Coding::TabRead::getFilePathes() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	return TabData::filePathes;
}

std::string FD::Coding::TabRead::getDisplayFilePath() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	return TabData::displayFile;
}

bool FD::Coding::TabRead::isDisplayFileChanged() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	const bool result = DisplayFileChanged;
	if (result)
		DisplayFileChanged = false;
	return result;
}

