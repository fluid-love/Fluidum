#include "../lua.h"

FluidumUtils_Debug_BeginDisableWarning(4646)
FS::Lua::Ret FS::Lua::Calc::terminate(State L) {
	throw std::runtime_error("Request Terminate Lua.");
}
FluidumUtils_Debug_EndDisableWarning
