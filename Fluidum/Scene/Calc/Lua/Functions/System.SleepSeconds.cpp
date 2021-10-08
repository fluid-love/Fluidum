#include "../lua.h"

FS::Lua::Ret FS::Lua::Calc::sleepSeconds(State L) {
	check.numOfArgs_min<Type::SleepSeconds>(L, 1);
	check.argType_intNum<Type::SleepSeconds>(L, { CoreType::Number });

	const Num sleepTimeSeconds = lua_tonumber(L, 1);
	auto time = std::chrono::duration<Num>(sleepTimeSeconds);

	std::this_thread::sleep_for(time);

	return 0;
}
