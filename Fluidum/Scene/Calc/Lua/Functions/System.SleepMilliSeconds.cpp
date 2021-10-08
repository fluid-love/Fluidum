#include "../lua.h"

FS::Lua::Ret FS::Lua::Calc::sleepMilliSeconds(State L) {
	check.numOfArgs_min<Type::SleepSeconds>(L, 1);
	check.argType_intNum<Type::SleepSeconds>(L, { CoreType::Number });

	const Num sleepTimeMilliSeconds = lua_tonumber(L, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<Val>(sleepTimeMilliSeconds)));

	return 0;
}
