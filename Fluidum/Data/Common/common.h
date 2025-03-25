#pragma once

#include "resource.h"
#include "log.h"
#include "passkey.h"

#include "../Limits/limits.h"
#include <boost/predef.h>
#include <boost/json.hpp>
#include <lua.hpp>

#include <filesystem>
#include <deque>
#include <variant>
#include <map>
#include <functional>

//forward
namespace FD {
	template<typename... T>
	class Manager;
}

namespace FD::Internal {
	class ManagerPassKey {
	public:
		template<typename...T>
		friend class ::FD::Manager;

	private:
		constexpr ManagerPassKey() = default;
		ManagerPassKey(const ManagerPassKey&) = delete;
		ManagerPassKey& operator=(const ManagerPassKey&) = delete;
		ManagerPassKey& operator=(ManagerPassKey&&) = delete;
		~ManagerPassKey() = default;
	public:
		constexpr ManagerPassKey(ManagerPassKey&&) = default;
	};

	using PassKey = const ManagerPassKey&;

}

namespace FD {
	
	using UT = UIF16;

}
