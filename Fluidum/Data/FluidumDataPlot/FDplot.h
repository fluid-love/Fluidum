#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <mutex>

#include "property/FDlimit.h"
#include "../common/FDcommon.h"

namespace FD {

	namespace Plot {

		enum class PlotType :uint16_t {
			NONE = 0,
			LINE = 1,
			BAR = 2,
		};

		enum class PlotMarker : uint16_t {
			CIRCLE = 0,
			SQUARE,    
			DIAMOND,   
			UP,        
			DOWN,      
			LEFT,      
			RIGHT,     
			CROSS,     
			PLUS,      
			ASTERISK,  
		};

		enum class Result :uint16_t {
			SUCCESS = 0x0000,
			FIGURE_INDEX_OVER = 0x0001,
			DATA_INDEX_OVER = 0x0002,
		};

		struct Figure final {
			std::string title = "NoName";//中央上
			std::string axisLabelX = "x";//下
			std::string axisLabelY = "y";//左
		};

		struct Data final {
			Plot::PlotType type = Plot::PlotType::LINE;//線なのか点なのか　バーなのか...
			Plot::PlotMarker marker = Plot::PlotMarker::CIRCLE;//点の形

			void setPlot(const double x, const double y) {
				valuesX.emplace_back(x);
				valuesY.emplace_back(y);
			}
			std::vector<Plot::Val> valuesX = {};
			std::vector<Plot::Val> valuesY = {};
		};

		//これ一つで1つの塊
		struct PlotData final {
			Figure figure;
			std::vector<Data> data;//描写する線
		};
	}


	class PlotWrite final {
	public:
		explicit PlotWrite(Internal::ManagerKey) {};
		~PlotWrite() = default;
		FluidumUtils_Class_NoCopyNoMove(PlotWrite)

	public:

		//戻り値は現在のPlotの個数-1 == index
		_NODISCARD Plot::FigureIndex addFigure(const char* title, const char* axisLabelX, const char* axisLabelY);

		_NODISCARD Plot::Result setPlotType(const Plot::FigureIndex figureIndex, const Plot::DataIndex dataIndex, const Plot::PlotType type);
	
		_NODISCARD Plot::Result setPlotMarker(const Plot::FigureIndex figureIndex, const Plot::DataIndex dataIndex, const Plot::PlotMarker marker) const noexcept;

		_NODISCARD Plot::Result addPlot(const Plot::FigureIndex plotIndex, const Plot::DataIndex dataIndex, const double x, const double y);


	};

	class PlotRead final {
	public:
		explicit PlotRead(Internal::ManagerKey) {};
		~PlotRead() = default;
		FluidumUtils_Class_NoCopyNoMove(PlotRead)

	public:
		_NODISCARD const std::vector<FD::Plot::PlotData>* get() const noexcept;

		void lock() const;
		
		void unlock() const;

	};



}