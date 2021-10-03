#include "../lua.h"

FS::Lua::Ret FS::LuaCalc::terminate(State L) {
	throw std::runtime_error("Request Terminate Lua.");
}

