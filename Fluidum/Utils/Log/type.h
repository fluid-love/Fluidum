#pragma once

#include <iostream>
#include <format>
#include <chrono>
#include <sstream>
#include <mutex>

#include "../Class/class.h"

namespace FU::Log {

	enum class Type : uint8_t {
		None,
		Info,
		Warning,
		Error
	};

	using MessengerCallbackType = void(*)(const std::string& message);
}