#pragma once

#include "../Common/type.h"
#include "../Common/fkm.h"

namespace FVK::Internal::Exception {

	[[noreturn]] void throwNotInitialized();
	[[noreturn]] void throwFailedToCreate();
	[[noreturn]] void throwFailedToDestroy();
	[[noreturn]] void throwNotFound();
	[[noreturn]] void throwNotSupported();
	[[noreturn]] void throwCollisionOfKeys();
	[[noreturn]] void throwUnexpected();
	[[noreturn]] void throwUnknown();

}

namespace FVK::Exception {

	enum class ErrorType : UIF8 {
		NotInitialized,
		FailedToCreate,
		FailedToDestroy,
		NotFound,
		NotSupported,
		CollisionOfKeys,
		Unexpected,
		Unknown
	};

	using Error = FU::Exception::Base<ErrorType>;

}