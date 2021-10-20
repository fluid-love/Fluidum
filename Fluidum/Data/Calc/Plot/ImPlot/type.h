#pragma once

#include "../../../Common/common.h"
#include "../../../Libraries/implot/implot.h"

namespace FD::Plot {

	enum class PlotType :uint16_t {
		None,
		Line,
		Bar,
	};

	struct Figure final {
		std::string title = "NoName";//top
		std::string axisLabelX = "x";//bottom
		std::string axisLabelY = "y";//left
	};

	struct Data final {
		Plot::PlotType type = Plot::PlotType::Line;//line ,point,bar...
		ImPlotMarker marker = ImPlotMarker_Circle;//circle,up,down...

		std::vector<Plot::Val> valuesX = {};
		std::vector<Plot::Val> valuesY = {};
	};

	struct PlotData final {
		Figure figure;
		std::vector<Data> data;//ï`é Ç∑ÇÈê¸
	};

}