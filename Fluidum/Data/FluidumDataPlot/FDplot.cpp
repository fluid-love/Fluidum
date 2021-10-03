#include "FDplot.h"

FD::Plot::Result operator|(const FD::Plot::Result l, const FD::Plot::Result r) {
	return static_cast<FD::Plot::Result>(static_cast<std::underlying_type_t<FD::Plot::Result>>(l) | static_cast<std::underlying_type_t<FD::Plot::Result>>(r));
}

void operator|=(FD::Plot::Result& l, const FD::Plot::Result r) {
	l = l | r;
}

//GPlot
std::vector<FD::Plot::PlotData> GPlot = {};
std::mutex GMtx;

FD::Plot::FigureIndex FD::PlotWrite::addFigure(const char* title, const char* axisLabelX, const char* axisLabelY) {
	std::lock_guard<std::mutex> lock(GMtx);

	Plot::PlotData plot = {};
	plot.figure.axisLabelX = axisLabelX;
	plot.figure.axisLabelY = axisLabelY;
	plot.figure.title = title;

	GPlot.emplace_back(std::move(plot));

	return static_cast<Plot::FigureIndex>(GPlot.size());
}

FD::Plot::Result FD::PlotWrite::setPlotType(const Plot::FigureIndex figureIndex, const Plot::DataIndex dataIndex, const Plot::PlotType type) {
	std::lock_guard<std::mutex> lock(GMtx);

	Plot::Result result = Plot::Result::SUCCESS;

	if (GPlot.size() < figureIndex)
		result |= Plot::Result::FIGURE_INDEX_OVER;

	if (GPlot[figureIndex].data.size() < dataIndex) {
		result |= Plot::Result::DATA_INDEX_OVER;
	}

	if (result == Plot::Result::SUCCESS) {
		GPlot[figureIndex].data[dataIndex].type = type;
	}

	return result;
}

FD::Plot::Result FD::PlotWrite::setPlotMarker(const Plot::FigureIndex figureIndex, const Plot::DataIndex dataIndex, const Plot::PlotMarker marker)const noexcept {
	std::lock_guard<std::mutex> lock(GMtx);

	Plot::Result result = Plot::Result::SUCCESS;

	if (GPlot.size() < figureIndex)
		result |= Plot::Result::FIGURE_INDEX_OVER;

	if (GPlot[figureIndex].data.size() < dataIndex) {
		result |= Plot::Result::DATA_INDEX_OVER;
	}

	if (result == Plot::Result::SUCCESS) {
		GPlot[figureIndex].data[dataIndex].marker = marker;
	}

	return result;
}

FD::Plot::Result FD::PlotWrite::addPlot(const Plot::FigureIndex plotIndex, const Plot::DataIndex dataIndex, const double x, const double y) {
	std::lock_guard<std::mutex> lock(GMtx);

	Plot::Result result = Plot::Result::SUCCESS;
	if (GPlot.size() <= plotIndex)
		result |= Plot::Result::FIGURE_INDEX_OVER;

	if (GPlot[plotIndex].data.size() <= dataIndex) {
		GPlot[plotIndex].data.resize(dataIndex + 1);

		//‚ ‚ç‚©‚¶‚ßŠm•Û‚µ‚Ä‚¨‚¢‚Äˆ—ŽžŠÔ‚ð’Zk‚·‚éD‚½‚¾vector‚É”C‚¹‚é•û‚ª—Ç‚¢ê‡‚à...
		GPlot[plotIndex].data[dataIndex].valuesX.reserve(1000);
		GPlot[plotIndex].data[dataIndex].valuesY.reserve(1000);
	}

	if (result == Plot::Result::SUCCESS) {
		GPlot[plotIndex].data[dataIndex].setPlot(x, y);
	}

	return result;
}

const std::vector<FD::Plot::PlotData>* FD::PlotRead::get() const noexcept {
	return &GPlot;
}

void FD::PlotRead::lock() const {
	GMtx.lock();
}

void FD::PlotRead::unlock() const {
	GMtx.unlock();
}