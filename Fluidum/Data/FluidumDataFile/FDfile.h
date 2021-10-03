#pragma once

#include <string>
#include <iostream>

#include "../common/FDcommon.h"

namespace FD::FilePath {

	constexpr inline const char* FluidumTempFile = "FluidumTempLog/FluidumTemp.flu";

}

namespace FD {

	class File final {
	public:
		File(Internal::ManagerKey);//directoryçÏê¨
		~File() = default;
		FluidumUtils_Class_NoCopyNoMove(File)

	private:

	};

}