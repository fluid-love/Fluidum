#pragma once

#include "type.h"

namespace FVK::Internal {

	inline std::mutex GMutex = {};

	using LockGuard = std::lock_guard<std::mutex>;
	using UniqueLock = std::unique_lock<std::mutex>;

}