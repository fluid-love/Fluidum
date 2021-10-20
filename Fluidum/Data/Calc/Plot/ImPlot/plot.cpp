#include "plot.h"

//FD::Plot::Result operator|(const FD::Plot::Result l, const FD::Plot::Result r) {
//	return static_cast<FD::Plot::Result>(static_cast<std::underlying_type_t<FD::Plot::Result>>(l) | static_cast<std::underlying_type_t<FD::Plot::Result>>(r));
//}
//
//void operator|=(FD::Plot::Result& l, const FD::Plot::Result r) {
//	l = l | r;
//}

namespace FD {
	class Index final {
	public:
		void operator++(int) noexcept {
			i++;
		}

		Plot::FigureIndex get() const noexcept {
			return this->i;
		}

		void reset() noexcept {
			this->i = 0;
		}
	private:
		Plot::FigureIndex i = 0;
	}GIndex;

	//GPlot
	std::map<Plot::FigureIndex, FD::Plot::PlotData> GPlot = {};
	std::mutex GMtx;
}


FD::Plot::FigureIndex FD::ImPlotWrite::addFigure(const char* title, const char* axisLabelX, const char* axisLabelY) {
	std::lock_guard<std::mutex> lock(GMtx);

	assert(FD::Plot::Limit::FigureMax >= GPlot.size());

	Plot::PlotData plot = {};
	plot.figure.axisLabelX = axisLabelX;
	plot.figure.axisLabelY = axisLabelY;
	plot.figure.title = title;

	GIndex++;
	GPlot.insert({ GIndex.get(),std::move(plot) });

	return GIndex.get();
}

//void FD::ImPlotWrite::setPlotType(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const Plot::PlotType type) {
//	std::lock_guard<std::mutex> lock(GMtx);
//
//	Plot::Result result = Plot::Result::SUCCESS;
//
//	if (GPlot.size() < figureIndex)
//		result |= Plot::Result::FIGURE_INDEX_OVER;
//
//	if (GPlot[figureIndex].data.size() < plotIndex) {
//		result |= Plot::Result::DATA_INDEX_OVER;
//	}
//
//	if (result == Plot::Result::SUCCESS) {
//		GPlot[figureIndex].data[plotIndex].type = type;
//	}
//
//	return result;
//}

void FD::ImPlotWrite::setMarker(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const ImPlotMarker marker) const noexcept {
	std::lock_guard<std::mutex> lock(GMtx);

	if (!GPlot.contains(figureIndex))
		throw std::runtime_error("Failed to set ImPlotMarker.");

	GPlot.at(figureIndex).data[plotIndex].marker = marker;
}

void FD::ImPlotWrite::pushBack(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const double x, const double y) {
	std::lock_guard<std::mutex> lock(GMtx);

	assert(GPlot.size() > plotIndex);

	auto& plot = GPlot.at(plotIndex);

	assert(FD::Plot::Limit::PlotMax >= plot.data.size());

	if (plot.data.size() <= plotIndex) {
		plot.data.resize(plotIndex + 1);

		plot.data[plotIndex].valuesX.reserve(1000);
		plot.data[plotIndex].valuesY.reserve(1000);
	}

	plot.data[plotIndex].valuesX.emplace_back(x);
	plot.data[plotIndex].valuesY.emplace_back(y);
}

const std::map<FD::Plot::FigureIndex, FD::Plot::PlotData>* FD::ImPlotRead::get() const {
	return &GPlot;
}

std::unique_lock<std::mutex> FD::ImPlotRead::getLock() const {
	return std::unique_lock<std::mutex>(GMtx);
}

FD::Plot::FigureIndex FD::ImPlotRead::figureSize() const {
	std::lock_guard<std::mutex> lock(GMtx);
	return GPlot.size();
}
