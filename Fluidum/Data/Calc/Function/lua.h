#pragma once

#include "container.h"

namespace FD::Calc::Lua {
	using Ret = int;
	using Num = LUA_NUMBER;//lua number
	using Val = long long;
	using String = const char*;//lua string
	using State = lua_State*;

	template<typename T>
	concept IsArgType =
		std::same_as<T, Num> &&
		std::same_as<T, Val> &&
		std::same_as<T, String>;

	enum class FunctionType : uint16_t {
		None = 0,
		Terminate,
		SleepSeconds,
		SleepMilliSeconds
	};

}

namespace FD::Calc::Lua::Internal {

	template<FunctionType>
	struct RetArgInfo final {
		FluidumUtils_Class_Delete_ConDestructor(RetArgInfo)
	};

	template<>
	struct RetArgInfo<FunctionType::Terminate> final {};

	template<>
	struct RetArgInfo<FunctionType::SleepSeconds> final {
		Num seconds{};
	};

	template<>
	struct RetArgInfo<FunctionType::SleepMilliSeconds> final {
		Num milliSeconds{};
	};


}