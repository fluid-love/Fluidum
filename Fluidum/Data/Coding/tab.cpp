#include "tab.h"

namespace FD::Internal::Coding {
	constexpr inline uint16_t LimitMaxFileSize = 1000;
	bool Update = false;
	bool Update_textSaved = false;
	bool DisplayFileChanged = false;

	struct FileInfo final {
		FTE::TextEditor editor{};
		bool isTextSaved = true;
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
	{
		auto itr = std::find(TabData::filePathes.begin(), TabData::filePathes.end(), path);
		if (itr == TabData::filePathes.end())
			throw Exception::NotFound;
		TabData::filePathes.erase(itr);
	}
	{
		auto itr = GData.find(path);
		if (itr != GData.end())
			GData.erase(itr);
	}

	this->update();
}

void FD::Coding::TabWrite::clear() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	TabData::displayFiles.clear();
	TabData::filePathes.clear();
	GData.clear();

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

void FD::Coding::TabWrite::releaseAllEditorData() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	GData.clear();
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

void FD::Coding::TabWrite::setIsTextSaved(const std::string& path, const bool val) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto* elm = GData.at(path).get();
	if (elm->isTextSaved == val)
		return;

	elm->isTextSaved = val;
	Update_textSaved = true;
}

void FD::Coding::TabWrite::setAllIsTextSaved(const bool val) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	for (auto& x : GData) {
		if (x.second.get()->isTextSaved != val) {
			x.second.get()->isTextSaved = val;
			Update_textSaved = true;
		}
	}
}

void FD::Coding::TabWrite::saveText(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	FileInfo* info = GData.at(path).get();

	//already saved
	if (info->isTextSaved)
		return;

	const std::string text = info->editor.GetText();
	std::ofstream ofs(path);
	ofs << text;

	info->isTextSaved = true;
}

void FD::Coding::TabWrite::saveAllTexts() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);

	for (auto& x : GData) {
		FileInfo* info = x.second.get();

		//already saved
		if (info->isTextSaved)
			continue;

		const std::string text = info->editor.GetText();
		std::ofstream ofs(x.first);
		ofs << text;

		info->isTextSaved = true;
	}
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

bool FD::Coding::TabRead::isTextSaved(const std::string& path) const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	return GData.at(path).get()->isTextSaved;
}

bool FD::Coding::TabRead::isAllTextSaved() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	auto itr = std::find_if(GData.begin(), GData.end(), [](auto& x) { return x.second.get()->isTextSaved; });
	return itr == GData.end();
}

bool FD::Coding::TabRead::update_isTextSaved() const {
	std::lock_guard<std::mutex> lock(TabData::mtx);
	const bool result = Update_textSaved;
	if (result)
		Update_textSaved = false;
	return result;
}

