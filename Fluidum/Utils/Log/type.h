#pragma once

#include <iostream>
#include <format>
#include <chrono>
#include <sstream>
#include <mutex>

#include "../Class/class.h"
#include "../Type/type.h"

namespace FU::Log {

	enum class Type : UIF8 {
		None,
		Info,
		Warning,
		Error
	};

	using MessengerCallbackType = void(*)(const std::string& message) noexcept;

}