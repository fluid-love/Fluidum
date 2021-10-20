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
		//FPlot PlotIndex��1�ȏ�łȂ���΂Ȃ�܂���D�w�肵���l{}�D
		Message message(LogType::Plot_PlotIndex_Min);
		GLog.add<FD::Log::Type::None>(message, plotIndex);
		throw Internal::Exception();
	}

	ImPlotMarker mkr{};
	try {
		mkr = toMarker(marker);
	}
	catch (const Internal::Exception) {
		//FPlot SetMarker�̈����ɖ����ȕ����񂪓n����܂����D�n���ꂽ������{}�D������������{}�D
		Message message(LogType::Plot_MarkerStr);
		GLog.add<FD::Log::Type::None>(message, marker, markerStr());

		std::rethrow_exception(std::current_exception());
	}

	if (plotIndex > FD::Plot::Limit::PlotMax) {
		//FPlot 1�̘g�ɕ`�ʂł���O���t�̍ő�l�𒴂���PlotIndex���ݒ肳��Ă��܂��D�ݒ肳�ꂽ�l{}�D�ő�l{}�D
		Message message(LogType::Plot_PlotIndex_Max);
		GLog.add<FD::Log::Type::None>(message, plotIndex, FD::Plot::Limit::PlotMax);

		std::rethrow_exception(std::current_exception());
	}

	implotWrite->setMarker(figureIndex, plotIndex, mkr);

	return 0;
}
