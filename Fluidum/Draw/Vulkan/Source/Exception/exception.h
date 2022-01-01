#pragma once

#include "../Common/type.h"
#include "glfw.h"
#include "../Common/fkm.h"

namespace FVK::Internal::Exception {

	[[noreturn]] void throwNotInitialized();
	[[noreturn]] void throwFailedToCreate();
	[[noreturn]] void throwFailedToDestroy();
	[[noreturn]] void throwNotFound();
	[[noreturn]] void throwNotSupported();
	[[noreturn]] void throwCollisionOfKeys();
	[[noreturn]] void throwUnexpected();

}

namespace FVK::Exception {

	enum class ErrorType : UIF8 {
		NotInitialized,
		FailedToCreate,
		FailedToDestroy,
		NotFound,
		NotSupported,
		CollisionOfKeys,
		Unexpected
	};

	class Error final {
	public:
		explicit Error(const ErrorType val) noexcept : val(val) {}

	public:
		[[nodiscard]] inline ErrorType code() const noexcept;

	private:
		const ErrorType val;
	};

}