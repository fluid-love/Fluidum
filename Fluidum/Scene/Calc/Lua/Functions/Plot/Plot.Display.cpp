#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_display(State L) {
	//figureIndex
	check.argType<Type::Plot_Display>(L, { LuAssist::Type::Integer });

	const Val figureIndex = lua_tointeger(L, 1);

	try {
		implotWrite->figureDisplay(static_cast<FD::Plot::FigureIndex>(figureIndex));
	}
	catch (const FD::ImPlotWrite::Exception value) {
		using enum FD::ImPlotWrite::Exception;

		if (value == FigureSize) {
			FluidumScene_Log_EnumClass_Error(value);
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}
	
	return 0;
}
