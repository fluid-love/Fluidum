#pragma once

#include "type.h"

namespace FKM::Exception {

	enum class ErrorType : UIF8 {
		CollisionOfKeys,
		ReachIndexLimit,
		NotFound,
		FailedToRemove
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

namespace FKM::Exception::Internal {

	[[noreturn]] void throwCollisionOfKeys();
	[[noreturn]] void throwReachIndexLimit();
	[[noreturn]] void throwNotFound();
	[[noreturn]] void throwFailedToRemove();

}