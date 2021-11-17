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
		[[nodiscard]] Plot::FigureIndex addFigure(const char* title, const char* axisLabelX, const char* axisLabelY) const;

		//return plot index
		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
			PlotSize   -> MaxSize
		*/
		[[nodiscard]] Plot::PlotIndex addPlot(const Plot::FigureIndex figureIndex) const;

		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
		*/
		void popBackPlot(const Plot::FigureIndex figureIndex) const;

		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
			PlotSize   -> MaxSize
			SameSize   -> dataX != dataY
		*/
		void setPlot(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, std::vector<Plot::Val>&& valuesX, std::vector<Plot::Val>&& valuesY) const;


		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
			PlotSize   -> plotIndex >= plots size
		*/
		void setMarker(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const ImPlotMarker marker) const;

		void pushBack(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex, const double x, const double y) const;

		/*
		Exception:
			FigureSize -> figureIndex >= GPlot.size()
		*/
		void figureDisplay(const Plot::FigureIndex figureIndex) const;
		void figureHide(const Plot::FigureIndex figureIndex) const;

	public:
		void clear() const;

	public:
		void reset() const;

	private:
		void checkFigureSize(const Plot::FigureIndex figureIndex) const;
		void checkFigureAndPlotSize(const Plot::FigureIndex figureIndex, const Plot::PlotIndex plotIndex) const;

	};

	class ImPlotRead_Lock final {
	public:
		explicit ImPlotRead_Lock(Internal::PassKey) noexcept {};
		~ImPlotRead_Lock() = default;
		FluidumUtils_Class_Delete_CopyMove(ImPlotRead_Lock)

	public:
		[[nodiscard]] Plot::FigureIndex figureSize() const;

		[[nodiscard]] const std::vector<FD::Plot::PlotData>* get() const;

		std::unique_lock<std::mutex> getLock() const;

	};

	class ImPlotRead final {
	public:
		explicit ImPlotRead(Internal::PassKey) noexcept {};
		~ImPlotRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ImPlotRead)

	public:
		[[nodiscard]] Plot::FigureIndex figureSize() const;

		[[nodiscard]] bool empty() const;


	};


}