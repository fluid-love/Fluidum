#include "lua.h"

void FD::Project::PropertyLuaWrite::entryFilePath(const std::string& path) {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	LuaData::entryFilePath = path;
}

std::string FD::Project::PropertyLuaRead::entryFilePath() const {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return LuaData::entryFilePath;
}

bool FD::Project::PropertyLuaRead::entryFileExists() const noexcept{
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//system_error
	return !LuaData::entryFilePath.empty();
}



















































