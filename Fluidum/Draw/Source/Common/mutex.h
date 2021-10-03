#pragma once

#include <mutex>

namespace FDR::Internal {
	inline std::mutex GMutex{};

	using LockGuard = std::lock_guard<std::mutex>;
}