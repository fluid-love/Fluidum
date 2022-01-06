#include "run.h"
#include "../Lua/lua.h"
#include "../../Utils/Data/reset.h"

FS::Calc::Run::Run(
	const FD::FluidumFilesRead* const fluidumFilesRead
) :
	fluidumFilesRead(fluidumFilesRead)
{
	FluidumScene_Log_Constructor(::FS::Calc::Run);
}

FS::Calc::Run::~Run() noexcept {
	FluidumScene_Log_Destructor(::FS::Calc::Run);
}

void FS::Calc::Run::call() {
	const FD::Project::File::SupportedFileType type = fluidumFilesRead->getCurrentMainCodeType();
	using enum FD::Project::File::SupportedFileType;

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (type == None) {
		this->deleteThis();
	}

	Scene::addScene<Utils::ResetData<FD::ImPlotWrite,FD::Calc::ArrayWrite>>();

	if (type == Lua) {
		FluidumScene_Log_RequestAddScene(::FS::Calc::Lua::Run);
		Scene::addAsyncScene<Calc::Lua::Run>();
	}
	else if (type == Python) {
		;
	}
	else if (type == Cpp) {
		;
	}
	else {
		FluidumScene_Log_InternalWarning();
	}

	this->deleteThis();
}

void FS::Calc::Run::deleteThis() {
	FluidumScene_Log_RequestDeleteScene(::FS::Calc::Run);
	Scene::deleteScene<Run>();
}
