#include "tab.h"

namespace FD::Internal::Coding {
	constexpr inline uint16_t LimitMaxFileSize = 1000;
	bool Update = false;
	bool DisplayFileChanged = false;

	struct FileInfo final {
		FTE::TextEditor editor{};
		bool isTextChanged = false;
	};

	std::map<std::string, std::unique_ptr<FileInfo>> GData;
}

using namespace ::FD::Internal::Coding;

void FD::Coding::TabWrite::addFile(const std::string& path) const {
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

void FD::Coding::TabWrite::eraseFile(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;
	TabData::filePathes.erase(itr);

	this->update();
}

void FD::Coding::TabWrite::addDisplayFile(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;

	if (!GData.contains(path)) {
		GData.insert({ path,std::make_unique<FileInfo>() });
	}

	TabData::displayFiles.emplace_back(*itr);

	DisplayFileChanged = true;
	this->update();
}

void FD::Coding::TabWrite::eraseDisplayFile(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;

	TabData::displayFiles.erase(itr);

	DisplayFileChanged = true;
	this->update();
}

FTE::TextEditor* FD::Coding::TabWrite::getEditor(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	return &GData.at(path).get()->editor;
}

void FD::Coding::TabWrite::update() const {
	Update = true;
}

void FD::Coding::TabWrite::save() const {
	TabData::save.store(true);
}

void FD::Coding::TabWrite::setIsTextChanged(const std::string& path, const bool val) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	GData.at(path).get()->isTextChanged = val;
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

std::vector<std::string> FD::Coding::TabRead::getDisplayFilePaths() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	return TabData::displayFiles;
}

bool FD::Coding::TabRead::isDisplayFileChanged() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	const bool result = DisplayFileChanged;
	if (result)
		DisplayFileChanged = false;
	return result;
}

