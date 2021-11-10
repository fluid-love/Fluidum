#pragma once

#include "type.h"

namespace FD {

	class ImPlotWrite final {
	public:
		explicit ImPlotWrite(Internal::PassKey) noexcept {};
		~ImPlotWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ImPlotWrite)

	public:
		enum class Exception : uint8_t {
			FigureSize,
			PlotSize,
			SameSize
		};

	public:

		//return fogure index
		/*
		Exception:
			FigureSize -> MaxSize
		*/
		_NODISCARD Plot::FigureIndex addFigure(const char* title, const char* axisLabelX, const char* axisLabelY) const;

		//return plot index
		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
			PlotSize   -> MaxSize
		*/
		_NODISCARD Plot::PlotIndex addPlot(const Plot::FigureIndex figureIndex) const;


		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
			PlotSize   -> MaxSize
			SameSize   -> dataX != dataY
		*/
		void addPlot(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, std::vector<Plot::Val>&& valuesX, std::vector<Plot::Val>&& valuesY) const;

		//void setPlotType(const Plot::FigureIndex figureIndex, const Plot::PlotIndex dataIndex, const Plot::ImPlotType type);

		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
			PlotSize   -> plotIndex >= plots size
		*/
		void setMarker(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const ImPlotMarker marker) const;

		void pushBack(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const double x, const double y) const;

	public:
		void clear() const;

	public:
		void reset() const;

	private:
		void checkFigureAndPlotSize(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex) const;

	};

	class ImPlotRead final {
	public:
		explicit ImPlotRead(Internal::PassKey) noexcept {};
		~ImPlotRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ImPlotRead)

	public:
		_NODISCARD Plot::FigureIndex figureSize() const;

		_NODISCARD const std::vector<FD::Plot::PlotData>* get() const;

		std::unique_lock<std::mutex> getLock() const;

	};



}