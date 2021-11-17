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
	const FD::Project::CodeType type = fluidumFilesRead->getCurrentMainCodeType();
	using enum FD::Project::CodeType;

	FU::Cursor::setCursorType(FU::Cursor::Type::Wait);

	if (type == Empty) {
		this->deleteThis();
	}
	else if (type == Error) {
		FluidumScene_Log_Abort();
		abort();
	}

	Scene::addScene<Utils::ResetData<FD::ImPlotWrite>>();

	if (type == Lua) {
		GLog.add<FD::Log::Type::None>("Request add Lua::CalcScene(Async).");
		Scene::addAsyncScene<Lua::Calc>();
	}
	else if (type == Python) {
		;
	}
	else if (type == AngelScript) {
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
