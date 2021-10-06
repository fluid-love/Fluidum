#include "tab.h"

namespace FD::Internal::Coding {
	constexpr inline uint16_t LimitMaxFileSize = 1000;
	bool Update = false;
}

using namespace ::FD::Internal::Coding;

void FD::Coding::TabWrite::addFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	if (TabData::filePathes.size() >= LimitMaxFileSize)
		throw Exception::LimitFileSizeMax;

	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr != TabData::filePathes.end())
		throw Exception::AlreadyExist;

	TabData::filePathes.emplace_back(path);
	Update = true;
}

void FD::Coding::TabWrite::eraseFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;
	TabData::filePathes.erase(itr);
	Update = true;
}

void FD::Coding::TabWrite::setDisplayFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw Exception::NotFound;
	TabData::displayFile = *itr;
	Update = true;
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


