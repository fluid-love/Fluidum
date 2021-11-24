#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::system_sleep_seconds(State L) {

	check.numOfArgs_min<Type::System_SleepSeconds>(L, 1);
	check.argType_intNum<Type::System_SleepSeconds>(L, { LuAssist::Type::Number });

	const Num sleepTimeSeconds = lua_tonumber(L, 1);
	auto time = std::chrono::duration<Num>(sleepTimeSeconds);

	std::this_thread::sleep_for(time);

	functionWrite->push<Type::System_SleepSeconds>(sleepTimeSeconds);
	return 0;
}
