#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_plot(State L) {
	//figure index, data index, marker
	check.argType<Type::Plot_Plot>(L, { CoreType::Table, CoreType::Table });

	const Val figureIndex = lua_tointeger(L, 1);
	const Val plotIndex = lua_tointeger(L, 2) - 1;
	

	return 0;
}
