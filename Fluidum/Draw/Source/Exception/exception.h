#pragma once

#include "../../../Utils/Exception/exception.h"

namespace FDR::Exception::Internal {

	[[noreturn]] void throwFailed();

}

namespace FDR::Exception {

	enum class ErrorType : UIF8 {
		Failed
	};

	using Error = FU::Exception::Base<ErrorType>;

}

