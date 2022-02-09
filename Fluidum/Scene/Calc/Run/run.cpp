#include "run.h"
#include "../Lua/lua.h"
#include "../../Utils/Data/reset.h"

FS::Calc::Run::Run(
	const FD::Project::PropertyRead* const propertyRead,
	const FD::Project::PropertyLuaRead* const propertyLuaRead,
	FD::Coding::TabWrite* const tabWrite,
	Info& info
) :
	propertyRead(propertyRead),
	propertyLuaRead(propertyLuaRead),
	tabWrite(tabWrite),

	info(info)
{
	FluidumScene_Log_Constructor(::FS::Calc::Run);

	this->run();
}

FS::Calc::Run::~Run() noexcept {
	FluidumScene_Log_Destructor(::FS::Calc::Run);
}

void FS::Calc::Run::call() {
	assert(false);
}

void FS::Calc::Run::run() {
	const FD::Project::Property::ProjectType type = propertyRead->currentProjectType();
	using enum FD::Project::Property::ProjectType;

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (type == None) {
		info.errorType = InfoErrorType::NotSetProjectType;
		return;
	}

	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Call constructor Utils::ResetData<FD::ImPlotWrite, FD::Calc::ArrayWrite>.");
	Scene::callConstructor<Utils::ResetData<FD::ImPlotWrite, FD::Calc::ArrayWrite>>();

	if (type == Lua) {
		if (!propertyLuaRead->entryFileExists()) {
			info.errorType = InfoErrorType::NotExistsEntryFile;
			return;
		}
		//save all texts
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Save all texts.");
		tabWrite->saveAllTexts();
		FluidumScene_Log_RequestAddScene(::FS::Calc::Lua::Run);
		Scene::addAsyncScene<Calc::Lua::Run>(propertyLuaRead->entryFilePath());
	}
	else if (type == Python) {
		;
	}
	else if (type == Cpp) {
		;
	}
	else {
		FluidumScene_Log_InternalWarning();
		info.errorType = InfoErrorType::InternalError;
		return;
	}

	info.errorType = InfoErrorType::NoError;
}
