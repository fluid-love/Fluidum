#pragma once

#include"../Concept/concept.h"

#include <exception>

namespace FU::Exception {

	[[noreturn]] inline void rethrow() {
		std::rethrow_exception(std::current_exception());
	}

}

namespace FU::Exception {

	template<::FU::Concept::IsScopedEnum ErrorType>
	class Base final {
	public:
		explicit Base(const ErrorType val) noexcept : val(val) {}

	public:
		[[nodiscard]] inline ErrorType code() const noexcept {
			return this->val;
		}

	private:
		const ErrorType val;

	};

}