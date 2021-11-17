#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_make(State L) {
	check.argType<Type::Plot_Make>(L, { LuAssist::Type::String,LuAssist::Type::String,LuAssist::Type::String });

	String title = lua_tostring(L, 1);
	String axisX = lua_tostring(L, 2);
	String axisY = lua_tostring(L, 3);

	FD::Plot::FigureIndex index;
	try {
		index = implotWrite->addFigure(title, axisX, axisY);
	}
	catch (const FD::ImPlotWrite::Exception value) {
		using enum FD::ImPlotWrite::Exception;

		if (value == FigureSize) {
			//ŠÖ”FPlot.Create‚ªŒÄ‚Î‚ê‚Ü‚µ‚½‚ªì¬‚Å‚«‚éFigure‚ÌÅ‘å”‚ğ’´‚¦‚Ü‚µ‚½DÅ‘å”{}D
			Message message(LogType::Plot_Figure_Max);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), FD::Plot::Limits::Plot::FigureMax);
			consoleWrite->add(std::move(log));
			throw Internal::Exception();
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}

	LuAssist::Utils::popAll(L);
	lua_pushinteger(L, index);
	return 1;
}
