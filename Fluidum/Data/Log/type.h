#pragma once

#include <chrono>
#include <sstream>
#include <fstream>
#include <deque>
#include <filesystem>
#include <mutex>
#include <format>
#include <cassert>

#include "../Common/common.h"

namespace FD::Internal::Log {
	
	//Create a directly under the executable using relative paths.
	constexpr inline const char* FluidumLogFolderDirectory = "FluidumLog/";

}