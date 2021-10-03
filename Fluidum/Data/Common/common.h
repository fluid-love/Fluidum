#pragma once

#include "resource.h"
#include <boost/predef.h>

#include <filesystem>
#include <deque>

//前方宣言
namespace FS {

	class Object;
	namespace ObjectPart {
		class Template;
	}

}

//前方宣言
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
