#include "../FSlua.h"

using namespace FS::LuaType;

template<>
class FS::LuaCheck::Check<Plot::MARKER> final : private Common<> {
public:
	using Common::Common;

private:
	const char* const funcName = "Plot.Marker";

public:
	void arg() {
		const auto types = LuaAssist::getCoreTypes(L);

		//ˆø”‚Ì”‚ª‚¨‚©‚µ‚¢
		if (types.size() != 3) {
			if (types.size() < 3) {
				//ŠÖ”{}‚Ìˆø”‚Ì”‚ª­‚È‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
				LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MIN> text;
				log->add<FD::LogType::ERROR>(text, funcName, types.size(), 3);
				throw std::runtime_error("");
			}
			else {
				//ŠÖ”{}‚Ìˆø”‚Ì”‚ª‘½‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
				LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
				log->add<FD::LogType::ERROR>(text, funcName, types.size(), 3);
				throw std::runtime_error("");
			}
		}

		for (uint16_t i = 0; i < 2; i++) {
			if (types[i] != CoreType::INTEGER && types[i] != CoreType::NUMBER) {
				//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				log->add<FD::LogType::ERROR>(text, funcName, i, LuaAssist::getCoreTypeName(types[i]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
				throw std::runtime_error("");
			}
		}


		if (types[2] != CoreType::STRING) {
			//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 3, LuaAssist::getCoreTypeName(types[2]), LuaAssist::getCoreTypeName(CoreType::STRING));
			throw std::runtime_error("");
		}

	}
};


FD::Plot::PlotMarker toMarker(const std::string str) {
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

	constexpr FD::Plot::PlotMarker results[] = {
		FD::Plot::PlotMarker::CIRCLE,
		FD::Plot::PlotMarker::SQUARE,
		FD::Plot::PlotMarker::DIAMOND,
		FD::Plot::PlotMarker::UP,
		FD::Plot::PlotMarker::DOWN,
		FD::Plot::PlotMarker::LEFT,
		FD::Plot::PlotMarker::RIGHT,
		FD::Plot::PlotMarker::CROSS,
		FD::Plot::PlotMarker::PLUS,
		FD::Plot::PlotMarker::ASTERISK
	};

	for (int16_t i = -2, j = 0; auto x : markers) {
		if (str == x) {
			return results[j];
		}
		if (i % 3 == 0)
			j++;
		i++;
	}

	throw std::runtime_error("");
}

Ret FS::Lua::marker_Plot(State L) {
	LuaCheck::Check<LuaType::Plot::MARKER> check(L, log);
	check.arg();

	const Val figureIndex = lua_tointeger(L, 1) - 1;
	const Val dataIndex = lua_tointeger(L, 2) - 1;
	String marker = lua_tostring(L, 3);

	const FD::Plot::PlotMarker mkr = toMarker(marker);

	auto result = plot->setPlotMarker(figureIndex, dataIndex, mkr);

	if (result != FD::Plot::Result::SUCCESS)
		throw std::runtime_error("");

	LuaAssist::pop(L);
	return 0;
}
