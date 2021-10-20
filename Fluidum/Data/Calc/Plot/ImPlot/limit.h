#pragma once

#include <iostream>

namespace FD::Plot {

	using FigureIndex = uint16_t;
	using PlotIndex = uint16_t;

	using Val = double;

}

namespace FD::Plot::Limit {

	constexpr inline uint8_t PlotMax = 20;
	constexpr inline uint8_t FigureMax = 20;

}