#pragma once

#include "../Type/type.h"

namespace FU::Exception {

	enum class TerminateExceptionType : UIF8 {
		Project
	};

	using TerminateFunctionType = void(*)(const TerminateExceptionType) noexcept;

}

namespace FU::Exception {

	inline TerminateFunctionType terminate = [](const TerminateExceptionType) noexcept {};

	inline void setTerminate(TerminateFunctionType func) noexcept {
		terminate = func;
	}

}

