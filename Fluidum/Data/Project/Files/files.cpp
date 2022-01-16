#include "files.h"
#include "../../Limits/limits.h"
#include "../Project/name.h"

void FD::ProjectFilesWrite_Lock::remove(const std::string& path) {
	using namespace Project::Internal;
	ProjectFilesData::projectFiles.erase(path);
}

bool FD::ProjectFilesWrite_Lock::tryChangeName(const std::string& path, const std::string& newName) {
	using namespace Project::Internal;

	const bool sameName = ProjectFilesData::projectFiles.sameName(path, newName);

	if (sameName) {
		return false;
	}

	ProjectFilesData::projectFiles.changePathAndName(path, newName);
	return true;
}

void FD::ProjectFilesWrite_Lock::sync(const std::string& top) {
	using namespace Project::Internal;

	//shallow sync
	ProjectFilesData::projectFiles.sync();

	auto* data = ProjectFilesData::projectFiles.get();

	using Info = Project::FileList::FileInfo;

	std::function<void(const std::string&)> func = [&](const std::string& parent) {

		for (const auto& entry : std::filesystem::directory_iterator(parent)) {
			const std::string name = entry.path().filename().string();
			std::string path = entry.path().string();
			path = FU::File::consistentDirectory(path);

			if (entry.is_directory()) {
				FU::File::tryPushSlash(path);
			}

			if (ProjectFilesRead_Lock::exists(path)) {
				if (entry.is_directory())
					func(path);
				continue;
			}

			if (entry.is_directory()) {
				Project::FileList::DirectoryInfo dir{};
				Info* elm = ProjectFilesData::projectFiles.add(parent, path, dir);
				func(elm->path);
			}
			else if (Project::File::isFileFormatSupported(name)) {
				Project::FileList::SupportedInfo supported{};
				Info* elm = ProjectFilesData::projectFiles.add(parent, path, supported);
			}
			else {
				Project::FileList::UnsupportedInfo unsupported{};
				Info* elm = ProjectFilesData::projectFiles.add(parent, path, unsupported);
			}

		}

	};

	for (const auto& entry : std::filesystem::directory_iterator(top)) {
		const std::string name = entry.path().filename().string();
		std::string path = entry.path().string();
		path = FU::File::consistentDirectory(path);

		if (entry.is_directory()) {
			FU::File::tryPushSlash(path);
		}

		if (ProjectFilesRead_Lock::exists(path)) {
			if (entry.is_directory())
				func(path);
			continue;
		}

		if (entry.is_directory()) {
			if (name == Project::Internal::Name::FluidumHiddenDirectory)
				continue;
			Project::FileList::DirectoryInfo dir{};

			Info* elm = ProjectFilesData::projectFiles.add({}, path, dir);
			func(elm->path);
		}
		else if (Project::File::isFileFormatSupported(name)) {
			Project::FileList::SupportedInfo supported{};
			Info* elm = ProjectFilesData::projectFiles.add({}, path, supported);
		}
		else {
			Project::FileList::UnsupportedInfo unsupported{};
			Info* elm = ProjectFilesData::projectFiles.add({}, path, unsupported);
		}

	}

}

void FD::ProjectFilesWrite_Lock::save() const {
	using namespace Project::Internal;
	ProjectFilesData::save.store(true);
}

std::vector<std::string> FD::ProjectFilesWrite_Lock::findOpenPaths() const {
	using namespace Project::Internal;
	std::vector<Project::FileList::FileInfo>* data = ProjectFilesData::projectFiles.get();

	using Info = Project::FileList::FileInfo;

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

void FD::ProjectFilesWrite_Lock::collapseAll() {
	using namespace Project::Internal;
	ProjectFilesData::projectFiles.forEach([](Project::FileList::FileInfo& info) {info.open = false; });
}

std::vector<FD::Project::FileList::FileInfo>* FD::ProjectFilesWrite_Lock::fileList() {
	using namespace Project::Internal;
	return ProjectFilesData::projectFiles.get();
}

std::unique_lock<std::mutex> FD::ProjectFilesWrite_Lock::getLock() {
	using namespace Project::Internal;
	return std::unique_lock<std::mutex>(ProjectFilesData::mtx);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FD::Size FD::ProjectFilesRead_Lock::numOfDirectories(const std::string& parent) const {
	using namespace Project::Internal;
	Size result = 0;
	ProjectFilesData::projectFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			if (info.type == Project::FileList::Type::Directory) {
				Size* count = static_cast<Size*>(data);
				(*count)++;
			}
		},
		&result);
	return result;
}

FD::Size FD::ProjectFilesRead_Lock::numOfFiles(const std::string& parent) const {
	using namespace Project::Internal;
	Size result = 0;
	ProjectFilesData::projectFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			if (info.type != Project::FileList::Type::Directory) {
				Size* count = static_cast<Size*>(data);
				(*count)++;
			}
		},
		&result);
	return result;
}

std::pair<bool, FD::Size> FD::ProjectFilesRead_Lock::childExists(const std::string& parent, const std::string& child) const {
	using namespace Project::Internal;
	if (parent.empty() || child.empty())
		return { false, 0 };
	return ProjectFilesData::projectFiles.childExists(parent, child);
}

FD::Project::FileList::FileInfo* FD::ProjectFilesRead_Lock::exists(const std::string& path) {
	using namespace Project::Internal;

	using UserData = std::tuple<bool, const std::string* const, Project::FileList::FileInfo*>;

	UserData result = std::make_tuple(false, &path, nullptr);

	ProjectFilesData::projectFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			UserData* res = static_cast<UserData*>(data);
			if (std::get<0>(*res))
				return;

			if (info.path == *(std::get<1>(*res))) {
				std::get<0>(*res) = true;
				std::get<2>(*res) = &info;
			}
		},
		&result);

	return std::get<2>(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::UserFilesWrite_Lock::remove(const std::string& path) {
	using namespace Project::Internal;
	UserFilesData::userFiles.erase(path);
}

bool FD::UserFilesWrite_Lock::tryChangeName(const std::string& path, const std::string& newName) {
	using namespace Project::Internal;

	const bool sameName = UserFilesData::userFiles.sameName(path, newName);

	if (sameName) {
		return false;
	}

	std::string newPath = path;
	UserFilesData::userFiles.changePathAndName(path, newName);
	return true;
}

std::vector<FD::Project::FileList::FileInfo>* FD::UserFilesWrite_Lock::fileList() {
	using namespace Project::Internal;
	return UserFilesData::userFiles.get();
}

std::unique_lock<std::mutex> FD::UserFilesWrite_Lock::getLock() {
	using namespace Project::Internal;
	return std::unique_lock<std::mutex>(UserFilesData::mtx);
}

void FD::UserFilesWrite_Lock::save() const {
	using namespace Project::Internal;
	UserFilesData::save.store(true);
}

void FD::UserFilesWrite_Lock::collapseAll() {
	using namespace Project::Internal;
	UserFilesData::userFiles.forEach([](Project::FileList::FileInfo& info) {info.open = false; });
}

void FD::UserFilesWrite_Lock::sync() {
	using namespace Project::Internal;
	UserFilesData::userFiles.sync();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::size_t FD::UserFilesRead_Lock::numOfVirtualFolder(const std::string& parent) const {
	using namespace Project::Internal;
	std::size_t result = 0;
	UserFilesData::userFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			if (info.type == Project::FileList::Type::Directory) {
				std::size_t* count = static_cast<std::size_t*>(data);
				(*count)++;
			}
		},
		&result);
	return result;
}

std::size_t FD::UserFilesRead_Lock::numOfFiles(const std::string& parent) const {
	using namespace Project::Internal;
	std::size_t result = 0;
	UserFilesData::userFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			if (info.type != Project::FileList::Type::Directory) {
				std::size_t* count = static_cast<std::size_t*>(data);
				(*count)++;
			}
		},
		&result);
	return result;
}

bool FD::UserFilesRead_Lock::exist(const std::string& path) const {
	using namespace Project::Internal;

	std::pair<bool, const std::string* const> result = std::make_pair(false, &path);

	UserFilesData::userFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			std::pair<bool, const std::string* const>* res = static_cast<std::pair<bool, const std::string* const>*>(data);
			if (res->first)
				return;

			if (info.path == (*res->second))
				res->first = true;
		},
		&result);

	return result.first;
}

bool FD::UserFilesRead_Lock::containForbiddenCharactor(const std::string& name) const {
	const bool result =
		(name.find('/') != std::string::npos) ||
		(name == "..") ||
		(name == ".");

	return result;
}

std::string FD::UserFilesRead_Lock::finalName(const std::string& name) const {
	assert(!containForbiddenCharactor(name) && !name.empty());
	//future
	return name;
}

std::string FD::UserFilesRead_Lock::rootDirectory() const {
	return "Fluidum:/";
}

std::string FD::UserFilesRead::rootDirectory() const {
	return "Fluidum:/";
}

bool FD::UserFilesRead::containForbiddenCharactor(const std::string& name) const {
	const bool result =
		(name.find('/') != std::string::npos) ||
		(name == "..") ||
		(name == ".");

	return result;
}

std::string FD::UserFilesRead::finalName(const std::string& name) const {
	assert(!containForbiddenCharactor(name) && !name.empty());
	//future
	return name;
}

std::string FD::UserFilesRead::canonical(const std::string& current, const std::string& path) const {
	assert(false);
	return{};
}

std::string FD::UserFilesRead::canonical(const std::string& path) const {
	assert(false);
	assert(path.find("Fluidum:/") == 0);

	std::string result = path;

	while (true) {
		{
			const auto pos = result.find("/./");
			if (pos != std::string::npos)
				result.replace(pos, pos + 3, "/");
		}

		//{
		//	const auto pos = result.find("/../");
		//	if (pos != std::string::npos) {
		//		const auto itr = std::find(result.begin() + pos, resu);
		//
		//	}
		//}
	}

	return{};
}

bool FD::UserFilesRead::exist(const std::string& path) const {
	using namespace Project::Internal;
	std::lock_guard<std::mutex> lock(UserFilesData::mtx);

	std::pair<bool, const std::string* const> result = std::make_pair(false, &path);

	UserFilesData::userFiles.forEach([](Project::FileList::FileInfo& info, void* data)
		{
			std::pair<bool, const std::string* const>* res = static_cast<std::pair<bool, const std::string* const>*>(data);
			if (res->first)
				return;

			if (info.path == (*res->second))
				res->first = true;
		},
		&result);

	return result.first;
}
