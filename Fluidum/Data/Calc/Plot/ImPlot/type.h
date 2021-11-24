#pragma once

#include "../../../Common/common.h"
#include "../../../External/implot/implot.h"

namespace FD::Plot {

	using FigureIndex = uint16_t;
	using PlotIndex = uint16_t;

	using Val = double;

}

namespace FD::Plot {

	enum class PlotType : uint8_t {
		Plot
	};

	enum class ImPlotType :uint8_t {
		None,
		Line,
		Point,
		Bar,
	};

	struct Figure final {
		std::string title = "NoName";//top
		std::string axisLabelX = "x";//bottom
		std::string axisLabelY = "y";//left
	};

	struct Data final {
		PlotType type = PlotType::Plot;
		Plot::ImPlotType imPlotType = Plot::ImPlotType::Point;//line ,point,bar...
		ImPlotMarker marker = ImPlotMarker_Circle;//circle,up,down...

		std::vector<Plot::Val> values1 = {};
		std::vector<Plot::Val> values2 = {};
	};

	struct PlotData final {
		Figure figure;
		std::vector<Data> plots;//ï`é Ç∑ÇÈê¸

		bool display = true;
	};

}