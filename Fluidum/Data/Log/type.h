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

	//100�܂ł��܂�����t�@�C���֏o��
	constexpr inline uint16_t LogArraySize = 100;

	//���΃p�X�Ŏ��s�t�@�C�������Ƀt�H���_���쐬
	constexpr inline const char* FluidumLogFolderDirectory = "FluidumLog/";

}