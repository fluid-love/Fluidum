#pragma once

#include "resource.h"
#include <boost/predef.h>
#include "../../Libraries/lua-5.4.3/src/lua.hpp"

#include <filesystem>
#include <deque>
#include <variant>
#include <map>

//‘O•ûéŒ¾
namespace FD {
	template<typename... T>
	class Manager;
}

namespace FD::Internal {
	class ManagerPassKey {
	public:
		template<typename...T>
		friend class ::FD::Manager;

#ifdef FluidumUtils_DebugMode
	public:
#else
	private:
#endif
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
