#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_pushBack(State L) {
	check.argType_intNum<Type::Plot_PushBack>(L, { CoreType::Integer,CoreType::Integer,CoreType::Number,CoreType::Number });

	Val figureKey = lua_tointeger(L, 1);
	Val plotIndex = lua_tointeger(L, 2) - 1;
	Num x = lua_tonumber(L, 3);
	Num y = lua_tonumber(L, 4);

	
	return 0;
}
