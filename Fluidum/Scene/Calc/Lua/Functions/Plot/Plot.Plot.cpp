#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_plot(State L) {
	//figure index, data index, marker
	check.argType<Type::Plot_Plot>(L,
		{
			LuAssist::Type::Integer,
			LuAssist::Type::Integer,
			LuAssist::Type::Table,//convertTableToArrayOfNumbers -> exception NotTable
			LuAssist::Type::Table //convertTableToArrayOfNumbers -> exception NotTable
		}
	);

	const Val figureIndex = lua_tointeger(L, 1);
	const Val plotIndex = lua_tointeger(L, 2);

	//x values
	std::vector<Num> x{};
	try {
		x = LuAssist::Utils::Table::convertTableToArrayOfNumbers(L, 3);
	}
	catch (const LuAssist::Utils::Table::Exception::NotNumber& info) {
		//{}ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚É“n‚³‚ê‚½ƒe[ƒuƒ‹‚Ì{}”Ô–Ú‚Ì—v‘f‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½Œ^:{}D³‚µ‚¢Œ^:{}D
		Message message(LogType::TableType);
		std::string log = GLog.add<FD::Log::Type::None>(
			message,
			LuAssist::Utils::getSrcCurrentLine(L, 2),
			"Plot",
			1,
			info.index,
			info.input,
			LuAssist::Utils::typeName(LuAssist::Type::Number)
			);
		consoleWrite->add(std::move(log));
		throw Internal::Exception();
	}

	//y values
	std::vector<Num> y{};
	try {
		y = LuAssist::Utils::Table::convertTableToArrayOfNumbers(L, 4);
	}
	catch (const LuAssist::Utils::Table::Exception::NotNumber& info) {
		//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚É“n‚³‚ê‚½ƒe[ƒuƒ‹‚Ì{}”Ô–Ú‚Ì—v‘f‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½Œ^:{}D³‚µ‚¢Œ^:{}D
		Message message(LogType::TableType);
		std::string log = GLog.add<FD::Log::Type::None>(
			message,
			LuAssist::Utils::getSrcCurrentLine(L, 2),
			"Plot",
			1,
			info.index,
			info.input,
			LuAssist::Utils::typeName(LuAssist::Type::Number)
			);
		consoleWrite->add(std::move(log));
		throw Internal::Exception();
	}

	//set
	try {
		implotWrite->setPlot(static_cast<FD::Plot::FigureIndex>(figureIndex), static_cast<FD::Plot::PlotIndex>(plotIndex), std::move(x), std::move(y));
	}
	catch (const FD::ImPlotWrite::Exception val) {
		using enum FD::ImPlotWrite::Exception;
		if (val == FigureSize || val == PlotSize) {
			FluidumScene_Log_EnumClass_Error(val);
		}
		else if (val == SameSize) {
			//{}ŠÖ”{}‚É“n‚³‚ê‚éx,y‚Ì—v‘f”‚Í“™‚µ‚­‚È‚¯‚ê‚Î‚È‚è‚Ü‚¹‚ñD
			Message message(LogType::Plot_SameSize);
			std::string log = GLog.add<FD::Log::Type::None>(
				message,
				LuAssist::Utils::getSrcCurrentLine(L, 2),
				"Plot"
				);
			consoleWrite->add(std::move(log));
			throw Internal::Exception();
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}

	return 0;
}
