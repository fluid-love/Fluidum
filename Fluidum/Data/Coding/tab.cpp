#include "tab.h"

using namespace ::FD::Internal::Coding;

void FD::Coding::TabWrite::addFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	TabData::filePathes.emplace_back(path);
}

void FD::Coding::TabWrite::eraseFile(const char* path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
	if (itr == TabData::filePathes.end())
		throw std::runtime_error("Not found.");
	TabData::filePathes.erase(itr);
}

std::vector<std::string> FD::Coding::TabRead::getFilePathes() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	return TabData::filePathes;
}



