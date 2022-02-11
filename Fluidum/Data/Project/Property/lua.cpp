#include "lua.h"

const char* FD::Project::Property::Lua::StandardFluidumLibraryTypeToCChar(const StandardFluidumLibrary val) noexcept {
	using enum StandardFluidumLibrary;
	if (val == System)
		return "system";
	if (val == Array)
		return "array";
	if (val == Plot)
		return "plot";
	if (val == Genome)
		return "genome";
	
	assert(false);
	return "";
}

void FD::Project::PropertyLuaWrite::entryFilePath(const std::string& path) {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	LuaData::entryFilePath = path;
}

void FD::Project::PropertyLuaWrite::enableStandardFluidumLibrary(const FD::Project::Property::Lua::StandardFluidumLibrary val) {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//std::system_error
	const auto find = std::find(
		LuaData::disabledFluidumStandardLibrary.cbegin(),
		LuaData::disabledFluidumStandardLibrary.cend(),
		val
	);

	if (find == LuaData::disabledFluidumStandardLibrary.cend()) {
		//already enabled
		return;
	}

	LuaData::disabledFluidumStandardLibrary.erase(find);
}

void FD::Project::PropertyLuaWrite::disableStandardFluidumLibrary(const Property::Lua::StandardFluidumLibrary val) {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//std::system_error
	const auto find = std::find(
		LuaData::disabledFluidumStandardLibrary.cbegin(),
		LuaData::disabledFluidumStandardLibrary.cend(),
		val
	);

	if (find != LuaData::disabledFluidumStandardLibrary.cend()) {
		//already disabled
		return;
	}

	LuaData::disabledFluidumStandardLibrary.emplace_back(val);

}

std::string FD::Project::PropertyLuaRead::entryFilePath() const {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return LuaData::entryFilePath;
}

bool FD::Project::PropertyLuaRead::entryFileExists() const noexcept {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//std::system_error
	return !LuaData::entryFilePath.empty();
}

std::vector<FD::Project::Property::Lua::StandardFluidumLibrary> FD::Project::PropertyLuaRead::disabledStandardFluidumLibrary() const {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//std::system_error
	return LuaData::disabledFluidumStandardLibrary;
}

bool FD::Project::PropertyLuaRead::isStandardFluidumLibraryDisabled(const Property::Lua::StandardFluidumLibrary val) const {
	using namespace Property::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);//std::system_error
	const auto find = std::find(
		LuaData::disabledFluidumStandardLibrary.cbegin(),
		LuaData::disabledFluidumStandardLibrary.cend(),
		val
	);
	return find != LuaData::disabledFluidumStandardLibrary.cend();
}



















































