#include "tab.h"

namespace FD::Coding::Internal {
	bool Update = false;
	bool Update_textSaved = false;
	bool DisplayFileChanged = false;

	struct FileInfo final {
		FTE::TextEditor editor{};
		bool isTextSaved = true;
	};

	std::map<std::string, std::unique_ptr<FileInfo>> GData;
}

void FD::Coding::TabWrite::add(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	if (Data::filePaths.size() >= Tab::Limits::FileSizeMax)
		throw Exception::LimitFileSizeMax;

	auto itr = std::find(Data::filePaths.begin(), Data::filePaths.end(), path);
	if (itr != Data::filePaths.end())
		throw Exception::AlreadyExist;

	if (!std::filesystem::exists(path))
		throw Exception::NotFound;

	Data::filePaths.emplace_back(path);

	this->update();
}

void FD::Coding::TabWrite::remove(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	{
		auto itr = std::find(Data::filePaths.begin(), Data::filePaths.end(), path);
		if (itr == Data::filePaths.end())
			throw Exception::NotFound;
		Data::filePaths.erase(itr);
	}
	{
		auto itr = GData.find(path);
		if (itr != GData.end())
			GData.erase(itr);
	}
	{
		auto itr = std::find(Data::displayFiles.begin(), Data::displayFiles.end(), path);
		if (itr != Data::displayFiles.end())
			Data::displayFiles.erase(itr);
	}

	this->update();
}

void FD::Coding::TabWrite::clear() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	Data::displayFiles.clear();
	Data::filePaths.clear();
	GData.clear();

	this->update();
}

void FD::Coding::TabWrite::releaseAllEditor() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	GData.clear();
}

FTE::TextEditor* FD::Coding::TabWrite::getEditor(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	auto itr = GData.find(path);
	if (itr == GData.end())
		throw Exception::NotFound;

	return &itr->second.get()->editor;
}

void FD::Coding::TabWrite::update() const {
	using namespace Internal;
	Update = true;
}

void FD::Coding::TabWrite::save() const {
	using namespace Internal;
	Data::save.store(true);
}

void FD::Coding::TabWrite::setIsTextSaved(const std::string& path, const bool val) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	auto* elm = GData.at(path).get();
	if (elm->isTextSaved == val)
		return;

	elm->isTextSaved = val;
	Update_textSaved = true;
}

void FD::Coding::TabWrite::setAllIsTextSaved(const bool val) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	for (auto& x : GData) {
		if (x.second.get()->isTextSaved != val) {
			x.second.get()->isTextSaved = val;
			Update_textSaved = true;
		}
	}
}

void FD::Coding::TabWrite::saveText(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	if (!GData.contains(path))
		throw Exception::NotFound;

	FileInfo* info = GData.at(path).get();

	//already saved
	if (info->isTextSaved)
		return;

	const std::string text = info->editor.GetText();
	std::ofstream ofs(path, std::ios::trunc);
	ofs << text;

	info->isTextSaved = true;
}

void FD::Coding::TabWrite::saveAllTexts() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

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
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	bool result = Update;
	if (result) {
		Update = false;
	}
	return result;
}

std::vector<std::string> FD::Coding::TabRead::paths() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return Data::filePaths;
}

bool FD::Coding::TabRead::isTextSaved(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return GData.at(path).get()->isTextSaved;
}

bool FD::Coding::TabRead::isAllTextSaved() const noexcept {
	using namespace Internal;
	try {
		std::lock_guard<std::mutex> lock(Data::mtx);//system_error
		auto itr = std::find_if(GData.begin(), GData.end(), [](auto& x) { return !x.second.get()->isTextSaved; });
		return itr == GData.end();
	}
	catch (const std::exception& e) {
		FluidumData_Log_Internal_StdExceptionError(e);
		std::terminate();
	}
	catch (...) {
		FluidumData_Log_Internal_InternalError();
		std::terminate();
	}
}

bool FD::Coding::TabRead::update_isTextSaved() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	const bool result = Update_textSaved;
	if (result)
		Update_textSaved = false;
	return result;
}

std::vector<std::string> FD::Coding::TabRead::notSavedTexts() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	std::vector<std::string> ret{};

	for (auto& x : GData) {
		if (!x.second.get()->isTextSaved)
			ret.emplace_back(x.first);
	}
	return ret;
}

bool FD::Coding::TabRead::exist(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);

	const auto itr = std::find(Data::filePaths.begin(), Data::filePaths.end(), path);
	return itr != Data::filePaths.end();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::Coding::DisplayWrite::add(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	auto itr = std::find(Data::filePaths.begin(), Data::filePaths.end(), path);
	if (itr == Data::filePaths.end())
		throw Exception::NotFound;

	if (!GData.contains(path)) {
		GData.insert({ path,std::make_unique<FileInfo>() });
	}

	Data::displayFiles.emplace_back(*itr);

	DisplayFileChanged = true;
}

void FD::Coding::DisplayWrite::remove(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	auto itr = std::find(Data::filePaths.begin(), Data::filePaths.end(), path);
	if (itr == Data::filePaths.end())
		throw Exception::NotFound;

	Data::displayFiles.erase(itr);

	DisplayFileChanged = true;
}

bool FD::Coding::DisplayWrite::tryRemove(const std::string& path) const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	auto itr = std::find(Data::filePaths.begin(), Data::filePaths.end(), path);
	if (itr == Data::filePaths.end())
		return false;

	Data::displayFiles.erase(itr);

	DisplayFileChanged = true;

	return true;
}

std::vector<std::string> FD::Coding::DisplayRead::paths() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return Data::displayFiles;
}

bool FD::Coding::DisplayRead::empty() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return Data::displayFiles.empty();
}

bool FD::Coding::DisplayRead::changed() const {
	using namespace Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	const bool result = DisplayFileChanged;
	if (result)
		DisplayFileChanged = false;
	return result;
}