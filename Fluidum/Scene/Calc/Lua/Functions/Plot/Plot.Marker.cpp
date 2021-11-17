#include "../../lua.h"

namespace FS {

	constexpr const char* markers[] = {
		"Circle","CIRCLE","circle",
		"Square","SQUARE","square",
		"Diamond","DIAMOND","diamond",
		"Up","UP","up",
		"Down","DOWN","down",
		"Left","LEFT","left",
		"Right","RIGHT","right",
		"Cross","CROSS","cross",
		"Plus","PLUS","plus",
		"Asterisk","ASTERISK","asterisk"
	};

	std::string markerStr() {
		std::string result{};
		result.reserve(300);
		for (auto x : markers) {
			result += x += ',';
		}
		return result;
	}

	ImPlotMarker toMarker(const std::string& str) {

		constexpr ImPlotMarker results[] = {
			ImPlotMarker_Circle,
			ImPlotMarker_Square,
			ImPlotMarker_Diamond,
			ImPlotMarker_Up,
			ImPlotMarker_Down,
			ImPlotMarker_Left,
			ImPlotMarker_Right,
			ImPlotMarker_Cross,
			ImPlotMarker_Plus,
			ImPlotMarker_Asterisk
		};

		for (int16_t i = -2, j = 0; auto x : markers) {
			if (str == x) {
				return results[j];
			}
			if (i % 3 == 0)
				j++;
			i++;
		}


		throw FS::Lua::Internal::Exception();
	}

}

FS::Lua::Ret FS::Lua::Calc::plot_setMarker(State L) {
	//figure index, data index, marker
	check.argType<Type::Plot_SetMarker>(L, { LuAssist::Type::Integer,LuAssist::Type::Integer,LuAssist::Type::String });

	const Val figureIndex = lua_tointeger(L, 1);
	const Val plotIndex = lua_tointeger(L, 2) - 1;
	String marker = lua_tostring(L, 3);

	assert(plotIndex >= 0 && plotIndex < std::numeric_limits<FD::Plot::PlotIndex>::max());
	assert(plotIndex >= 0 && plotIndex < std::numeric_limits<FD::Plot::PlotIndex>::max());

	ImPlotMarker mkr;
	try {
		mkr = toMarker(marker);
	}
	catch (const Internal::Exception) {
		//{}ä÷êîSetMarkerÇÃà¯êîÇ…ñ≥å¯Ç»ï∂éöóÒÇ™ìnÇ≥ÇÍÇ‹ÇµÇΩÅDìnÇ≥ÇÍÇΩï∂éöóÒ{}ÅDê≥ÇµÇ¢ï∂éöóÒ{}ÅD
		Message message(LogType::Plot_MarkerStr);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), marker, markerStr());
		consoleWrite->add(std::move(log));
		std::rethrow_exception(std::current_exception());
	}

	try {
		implotWrite->setMarker(static_cast<FD::Plot::FigureIndex>(figureIndex), static_cast<FD::Plot::PlotIndex>(plotIndex), mkr);
	}
	catch (const FD::ImPlotWrite::Exception value) {
		using enum FD::ImPlotWrite::Exception;

		if (value == FigureSize || value == PlotSize) {
			FluidumScene_Log_EnumClass_Error(value);
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}

	return 0;
}
