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

namespace FD::Log {
	using FU::Log::Type;
}

namespace FD::Internal::Log {

	//100までたまったらファイルへ出力
	constexpr inline uint16_t LogArraySize = 100;

	//相対パスで実行ファイル直下にフォルダを作成
	constexpr inline const char* FluidumLogFolderDirectory = "FluidumLog/";

}