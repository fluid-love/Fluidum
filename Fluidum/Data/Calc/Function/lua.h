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
		std::same_as<FU::Concept::remove_all_t<T>, Num> ||
		std::same_as<FU::Concept::remove_all_t<T>, Val> ||
		std::same_as<FU::Concept::remove_all_t<T>, String>;

	enum class FunctionType : uint16_t {
		None = 0,

		System_Terminate,
		System_SleepSeconds,
		System_SleepMilliSeconds,

		Plot_Make,
		Plot_makePlot,
		Plot_Plot,
		Plot_SetMarker,
		Plot_PushBack,
		Plot_PushFront
	};

}

namespace FD::Calc::Lua::Internal {

	template<FunctionType>
	struct RetArgInfo final {
		FluidumUtils_Class_Delete_ConDestructor(RetArgInfo)
	};

	template<>
	struct RetArgInfo<FunctionType::System_Terminate> final {};

	template<>
	struct RetArgInfo<FunctionType::System_SleepSeconds> final {
		Num seconds{};
	};

	template<>
	struct RetArgInfo<FunctionType::System_SleepMilliSeconds> final {
		Num milliSeconds{};
	};




}

#define FluidumData_Lua_ContainerTemplateArgs \
	::FD::Calc::Lua::Internal::RetArgInfo<::FD::Calc::Lua::FunctionType::System_Terminate>,\
	::FD::Calc::Lua::Internal::RetArgInfo<::FD::Calc::Lua::FunctionType::System_SleepSeconds>,\
	::FD::Calc::Lua::Internal::RetArgInfo<::FD::Calc::Lua::FunctionType::System_SleepMilliSeconds>\
