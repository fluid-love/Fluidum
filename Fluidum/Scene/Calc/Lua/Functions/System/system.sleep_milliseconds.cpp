#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::system_sleep_milliseconds(State L) {
	check.numOfArgs_min<Type::System_SleepMilliSeconds>(L, 1);
	check.argType_intNum<Type::System_SleepMilliSeconds>(L, { LuAssist::Type::Number });

	const Num sleepTimeMilliSeconds = lua_tonumber(L, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<Val>(sleepTimeMilliSeconds)));

	functionWrite->push<Type::System_SleepMilliSeconds>(sleepTimeMilliSeconds);

	return 0;
}
