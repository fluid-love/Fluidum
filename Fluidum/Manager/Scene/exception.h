#pragma once

#include <exception>
#include <string>

namespace FS::Exception {

	enum class ErrorType : UIF8 {
		AlreadyAdded,
		AlreadyDeleted,
	};

	using Error = FU::Exception::Base<ErrorType>;

	namespace Internal {

		[[noreturn]] inline void throwAlreadyAdded() {
			throw Error(ErrorType::AlreadyAdded);
		}

		template<typename Scene>
		[[noreturn]] inline void throwAlreadyDeleted() {
			throw Error(ErrorType::AlreadyDeleted);
		}

	}

}