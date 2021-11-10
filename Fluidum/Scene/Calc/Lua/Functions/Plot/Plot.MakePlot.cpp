#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_makePlot(State L) {
	check.argType<Type::Plot_makePlot>(L, { CoreType::Integer });

	Val figureIndex = lua_tointeger(L, 1);

	assert(figureIndex >= 0 && figureIndex < std::numeric_limits<FD::Plot::FigureIndex>::max());

	FD::Plot::PlotIndex index;

	try {
		index = implotWrite->addPlot(static_cast<FD::Plot::FigureIndex>(figureIndex));
	}
	catch (const FD::ImPlotWrite::Exception value) {
		using enum FD::ImPlotWrite::Exception;

		if (value == FigureSize) {
			FluidumScene_Log_EnumClass_Error(value);
		}
		else if (value == PlotSize) {
			//FPlot ŠÖ”FPlot.Create.MakePlot‚ªŒÄ‚Î‚ê‚Ü‚µ‚½‚ªì¬‚Å‚«‚éPlot‚ÌÅ‘å”‚ğ’´‚¦‚Ü‚µ‚½DÅ‘å”{}D
			Message message(LogType::Plot_Figure_Max);
			std::string log = GLog.add<FD::Log::Type::None>(message, FD::Plot::Limits::Plot::PlotMax);
			consoleWrite->add(std::move(log));
			throw Internal::Exception();
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}

	Lua::pop(L);
	lua_pushinteger(L, index);
	return 1;
}
