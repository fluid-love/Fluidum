#include "check_path.h"
#include "../Utils/Popup/message.h"

FS::File::Add::CheckPath::CheckPath(
	const FD::ProjectRead* const projectRead,
	const FD::UserFilesRead* const userFilesRead,
	Info& info
) :
	projectRead(projectRead),
	userFilesRead(userFilesRead),
	info(info)
{
	FluidumScene_Log_Constructor(::FS::File::Add::CheckPath);

	if (info.project)
		this->project();
	else
		this->user();
}

FS::File::Add::CheckPath::~CheckPath() {
	FluidumScene_Log_Destructor(::FS::File::Add::CheckPath);
}

void FS::File::Add::CheckPath::call() {
	assert(false && "Constructor");
}

void FS::File::Add::CheckPath::project() {

	std::string parent = FU::File::consistentDirectory(info.parent);
	FU::File::tryPushSlash(parent);

	//empty
	if (info.name.empty()) {
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_fill, info.pos_name);
		info.noerror = false;
		return;
	}

	//forbidden charactor
	if (FU::File::containForbiddenCharactor(info.name)) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Name({}) contains forbidden characters.", info.name);
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_forbiddenCharactor, info.pos_name);
		info.noerror = false;
		return;
	}

	std::string path{};
	{
		const std::filesystem::path par = parent;
		if (par.is_absolute()) {
			path = std::filesystem::weakly_canonical(par.string() + userFilesRead->finalName(info.name)).string();
			path = FU::File::consistentDirectory(path);
		}
		else { //relative
			path = projectRead->projectDirectoryPath() + parent + userFilesRead->finalName(info.name);
			path = std::filesystem::weakly_canonical(path).string();
			path = FU::File::consistentDirectory(path);
		}

		if (info.directory)
			FU::File::tryPushSlash(path);
	}

	if (std::filesystem::exists(path)) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Directory path({}) already exists.", path);
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_directoryAlreadyExist, info.pos_create);
		info.noerror = false;
		return;
	}

	info.noerror = true;
	info.fullPath = std::move(path);
}

void FS::File::Add::CheckPath::user() {

	std::string parent = FU::File::consistentDirectory(info.parent);
	FU::File::tryPushSlash(parent);

	//empty
	if (info.name.empty()) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Name is empty.");
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_fill, info.pos_name);
		info.noerror = false;
		return;
	}
	//parent.empty -> current path

	//forbidden charactor
	if (FU::File::containForbiddenCharactor(info.name) || userFilesRead->containForbiddenCharactor(info.name)) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Name({}) contains forbidden characters.", info.name);
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_forbiddenCharactor, info.pos_name);
		info.noerror = false;
		return;
	}

	std::string path{};
	std::string absoluteParentPath = parent;
	{
		const std::filesystem::path par = parent;
		if (par.is_absolute()) {
			path = std::filesystem::weakly_canonical(parent + userFilesRead->finalName(info.name)).string();
			path = FU::File::consistentDirectory(path);
		}
		else { //relative
			absoluteParentPath = projectRead->projectDirectoryPath() + parent;
			path = projectRead->projectDirectoryPath() + parent + userFilesRead->finalName(info.name);
			path = std::filesystem::weakly_canonical(path).string();
			path = FU::File::consistentDirectory(path);
		}

		if (info.directory)
			FU::File::tryPushSlash(path);
	}

	//parent does not exist
	if (!info.directory) {//! Fluidum:/
		if (!std::filesystem::exists(absoluteParentPath)) {
			GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Parent directory path({}) does not exist.", info.parent);
			FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
			Scene::addScene<Utils::Message>(text.error_parentDoesNotExist, info.pos_path);
			info.noerror = false;
			return;
		}
	}

	bool result = false;
	if (info.directory) {
		result = this->user_directory(path);
	}
	else {
		result = this->user_file(path);
	}
	if (!result)
		return;

	info.noerror = true;
	info.fullPath = std::move(path);
}

bool FS::File::Add::CheckPath::user_directory(const std::string& path) {
	if (userFilesRead->exist(path)) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Directory path({}) already exists.", path);
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_directoryAlreadyExist, info.pos_create);
		info.noerror = false;
		return false;
	}
	return true;
}

bool FS::File::Add::CheckPath::user_file(const std::string& path) {
	if (std::filesystem::exists(path)) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Directory path({}) already exists.", path);
		FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
		Scene::addScene<Utils::Message>(text.error_fileAlreadyExist, info.pos_create);
		info.noerror = false;
		return false;
	}
	return true;
}