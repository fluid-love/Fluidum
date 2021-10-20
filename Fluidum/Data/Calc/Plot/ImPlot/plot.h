#pragma once

#include "limit.h"
#include "type.h"

namespace FD {

	class ImPlotWrite final {
	public:
		explicit ImPlotWrite(Internal::PassKey) noexcept {};
		~ImPlotWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ImPlotWrite)

	public:

		//ñﬂÇËílÇÕåªç›ÇÃPlotÇÃå¬êî-1 == index
		_NODISCARD Plot::FigureIndex addFigure(const char* title, const char* axisLabelX, const char* axisLabelY);

		//void setPlotType(const Plot::FigureIndex figureIndex, const Plot::PlotIndex dataIndex, const Plot::PlotType type);

		void setMarker(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const ImPlotMarker marker) const noexcept;

		void pushBack(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const double x, const double y);


	};

	class ImPlotRead final {
	public:
		explicit ImPlotRead(Internal::PassKey) noexcept {};
		~ImPlotRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ImPlotRead)

	public:
		_NODISCARD Plot::FigureIndex figureSize() const;

		_NODISCARD const std::map<Plot::FigureIndex, FD::Plot::PlotData>* get() const;

		std::unique_lock<std::mutex> getLock() const;

	};



}