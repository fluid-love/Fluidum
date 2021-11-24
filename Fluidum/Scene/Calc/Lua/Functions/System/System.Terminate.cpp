#include "../../lua.h"

FluidumUtils_Debug_BeginDisableWarning(4646)
FS::Lua::Ret FS::Lua::Calc::system_terminate(State L) {
	functionWrite->push<Type::System_Terminate>();
	throw std::runtime_error("Request Terminate Lua.");
}
FluidumUtils_Debug_EndDisableWarning
