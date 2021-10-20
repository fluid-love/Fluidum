#include "../lua.h"

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
	check.argType<Type::Plot_SetMarker>(L, { CoreType::Integer,CoreType::Integer,CoreType::String });

	const Val figureIndex = lua_tointeger(L, 1);
	const Val plotIndex = lua_tointeger(L, 2) - 1;
	String marker = lua_tostring(L, 3);

	assert(figureIndex >= 0);

	if (plotIndex <= 0) {
		//FPlot PlotIndexは1以上でなければなりません．指定した値{}．
		Message message(LogType::Plot_PlotIndex_Min);
		GLog.add<FD::Log::Type::None>(message, plotIndex);
		throw Internal::Exception();
	}

	ImPlotMarker mkr{};
	try {
		mkr = toMarker(marker);
	}
	catch (const Internal::Exception) {
		//FPlot SetMarkerの引数に無効な文字列が渡されました．渡された文字列{}．正しい文字列{}．
		Message message(LogType::Plot_MarkerStr);
		GLog.add<FD::Log::Type::None>(message, marker, markerStr());

		std::rethrow_exception(std::current_exception());
	}

	if (plotIndex > FD::Plot::Limit::PlotMax) {
		//FPlot 1つの枠に描写できるグラフの最大値を超えたPlotIndexが設定されています．設定された値{}．最大値{}．
		Message message(LogType::Plot_PlotIndex_Max);
		GLog.add<FD::Log::Type::None>(message, plotIndex, FD::Plot::Limit::PlotMax);

		std::rethrow_exception(std::current_exception());
	}

	implotWrite->setMarker(figureIndex, plotIndex, mkr);

	return 0;
}
