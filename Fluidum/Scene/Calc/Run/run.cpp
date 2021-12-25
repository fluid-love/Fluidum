#include "run.h"
#include "../Lua/lua.h"
#include "../../Utils/Data/reset.h"

FS::Calc::Run::Run(
	const FD::FluidumFilesRead* const fluidumFilesRead
) :
	fluidumFilesRead(fluidumFilesRead)
{
	GLog.add<FD::Log::Type::None>("Construct Calc::RunScene.");
}

FS::Calc::Run::~Run() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Calc::RunScene.");
	}
	catch (const std::exception& e) {
		try {
			std::cerr << e.what() << std::endl;
			abort();
		}
		catch (...) {
			abort();
		}
	}
	catch (...) {
		abort();
	}
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
		FluidumScene_Log_RequestAddScene("Lua::Calc");
		Scene::addAsyncScene<Lua::Calc>();
	}
	else if (type == Python) {
		;
	}
	else if (type == Cpp) {
		;
	}
	else {
		FluidumScene_Log_Abort();
		abort();
	}

	this->deleteThis();
}

void FS::Calc::Run::deleteThis() {
	FluidumScene_Log_RequestDeleteScene("Calc::RunScene");
	Scene::deleteScene<Run>();
}
