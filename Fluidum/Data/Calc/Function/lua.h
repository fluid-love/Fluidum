#pragma once

#include "container.h"
#include <LuAssist/type.h>

namespace FD::Calc::Lua {
	using Ret = LuAssist::Ret;
	using Num = LuAssist::Num;
	using Val = LuAssist::Val;
	using String = LuAssist::String;
	using State = LuAssist::State;

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

		array_make,

		Plot_Make,
		Plot_makePlot,
		Plot_Plot,
		Plot_SetMarker,
		Plot_PushBack,
		Plot_PushFront,
		Plot_Display,
		Plot_Hide,
	};

}

namespace FD::Calc::Lua::Internal {

	template<FunctionType>
	struct RetArgInfo final {
		FluidumUtils_Class_Delete_ConDestructor(RetArgInfo);
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
