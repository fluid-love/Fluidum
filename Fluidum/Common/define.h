#pragma once

namespace Fluidum::Version {

	inline constexpr unsigned int Major = 1u;
	inline constexpr unsigned int Minor = 0u;
	inline constexpr unsigned int Patch = 0u;

	[[nodiscard]] inline constexpr const char* constChar() noexcept {
		return "1.0.0";
	}

}