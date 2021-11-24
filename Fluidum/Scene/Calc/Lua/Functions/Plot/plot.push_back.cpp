#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_push_back(State L) {

	check.argType_intNum<Type::Plot_PushBack>(L,
		{
			LuAssist::Type::Integer,
			LuAssist::Type::Integer,
			LuAssist::Type::Number,
			LuAssist::Type::Number
		}
	);

	Val figureKey = lua_tointeger(L, 1);
	Val plotIndex = lua_tointeger(L, 2) - 1;
	Num x = lua_tonumber(L, 3);
	Num y = lua_tonumber(L, 4);


	return 0;
}