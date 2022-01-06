#pragma once

#include "../../Common/messenger.h"

namespace FS::Calc::Lua::Internal {

	//This is thrown when a user error occurs.
	class Exception final {};

	//This is thrown when Fluidum error occurs.
	class InternalError final {};

}

namespace FS::Calc::Lua::Internal {

	[[noreturn]] inline void throwException() {
		throw Exception();
	}

	[[noreturn]] inline void throwInternalError() {
		throw InternalError();
	}

}