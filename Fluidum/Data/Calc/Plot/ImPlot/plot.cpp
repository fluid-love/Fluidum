#include "plot.h"
#include "../../../Limits/limits.h"

namespace FD {

	//GPlot
	std::vector<FD::Plot::PlotData> GPlot = {};
	std::mutex GMtx;
}

FD::Plot::FigureIndex FD::ImPlotWrite::addFigure(const char* title, const char* axisLabelX, const char* axisLabelY) const {
	std::lock_guard<std::mutex> lock(GMtx);

	if (GPlot.size() >= FD::Plot::Limits::Plot::FigureMax)
		throw Exception::FigureSize;

	Plot::PlotData plot = {};
	plot.figure.axisLabelX = axisLabelX;
	plot.figure.axisLabelY = axisLabelY;
	plot.figure.title = title;

	GPlot.emplace_back(std::move(plot));
	return static_cast<Plot::FigureIndex>(GPlot.size() - 1);
}

//void FD::ImPlotWrite::setPlotType(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const Plot::ImPlotType type) {
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

FD::Plot::PlotIndex FD::ImPlotWrite::addPlot(const Plot::FigureIndex figureIndex)const {
	std::lock_guard<std::mutex> lock(GMtx);

	this->checkFigureSize(figureIndex);

	if (GPlot[figureIndex].plots.size() >= FD::Plot::Limits::Plot::PlotMax)
		throw Exception::PlotSize;

	GPlot[figureIndex].plots.emplace_back();

	return static_cast<Plot::PlotIndex>(GPlot[figureIndex].plots.size() - 1);
}

void FD::ImPlotWrite::popBackPlot(const Plot::FigureIndex figureIndex) const {
	std::lock_guard<std::mutex> lock(GMtx);

	this->checkFigureSize(figureIndex);

	assert(GPlot[figureIndex].plots.empty() == false);

	GPlot[figureIndex].plots.pop_back();
}

void FD::ImPlotWrite::setPlot(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, std::vector<Plot::Val>&& valuesX, std::vector<Plot::Val>&& valuesY) const {
	std::lock_guard<std::mutex> lock(GMtx);

	this->checkFigureAndPlotSize(figureIndex, plotIndex);

	if (valuesX.size() != valuesY.size())
		Exception::SameSize;

	GPlot[figureIndex].plots[plotIndex].values1 = std::move(valuesX);
	GPlot[figureIndex].plots[plotIndex].values2 = std::move(valuesY);

}

void FD::ImPlotWrite::setMarker(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const ImPlotMarker marker) const {
	std::lock_guard<std::mutex> lock(GMtx);

	this->checkFigureAndPlotSize(figureIndex, plotIndex);

	GPlot[figureIndex].plots[plotIndex].marker = marker;
}

void FD::ImPlotWrite::pushBack(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const double x, const double y) const {
	std::lock_guard<std::mutex> lock(GMtx);

	assert(GPlot.size() > plotIndex);

	auto& plot = GPlot.at(plotIndex);

	//assert(FD::Plot::Limit::PlotMax >= plot.data.size());

	//if (plot.data.size() <= plotIndex) {
	//	plot.data.resize(plotIndex + 1);

	//	plot.data[plotIndex].values1.reserve(1000);
	//	plot.data[plotIndex].values2.reserve(1000);
	//}


	//plot.data[plotIndex].values1.emplace_back(x);
	//plot.data[plotIndex].values2.emplace_back(y);
}

void FD::ImPlotWrite::figureDisplay(const Plot::FigureIndex figureIndex) const {
	std::lock_guard<std::mutex> lock(GMtx);
	this->checkFigureSize(figureIndex);

	GPlot[figureIndex].display = true;
}

void FD::ImPlotWrite::figureHide(const Plot::FigureIndex figureIndex) const {
	std::lock_guard<std::mutex> lock(GMtx);
	this->checkFigureSize(figureIndex);

	GPlot[figureIndex].display = false;
}

void FD::ImPlotWrite::reset() const {
	std::lock_guard<std::mutex> lock(GMtx);
	GPlot.clear();
	GPlot.shrink_to_fit();
}

void FD::ImPlotWrite::checkFigureSize(const Plot::FigureIndex figureIndex) const {
	if (figureIndex >= GPlot.size())
		throw Exception::FigureSize;
}

void FD::ImPlotWrite::checkFigureAndPlotSize(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex) const {
	if (figureIndex >= GPlot.size())
		throw Exception::FigureSize;
	if (plotIndex >= GPlot[figureIndex].plots.size())
		throw Exception::PlotSize;
}

const std::vector<FD::Plot::PlotData>* FD::ImPlotRead_Lock::get() const {
	return &GPlot;
}

std::unique_lock<std::mutex> FD::ImPlotRead_Lock::getLock() const {
	return std::unique_lock<std::mutex>(GMtx);
}

FD::Plot::FigureIndex FD::ImPlotRead_Lock::figureSize() const {
	return static_cast<Plot::FigureIndex>(GPlot.size());
}

FD::Plot::FigureIndex FD::ImPlotRead::figureSize() const {
	std::lock_guard<std::mutex> lock(GMtx);
	return static_cast<Plot::FigureIndex>(GPlot.size());
}

bool FD::ImPlotRead::empty() const {
	std::lock_guard<std::mutex> lock(GMtx);
	return GPlot.empty();
}




