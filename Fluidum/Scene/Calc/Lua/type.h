#pragma once

#include "../../Common/common.h"

namespace FS::Lua {

	using Ret = FD::Calc::Lua::Ret;
	using Num = FD::Calc::Lua::Num;//lua number
	using Val = FD::Calc::Lua::Val;
	using String = FD::Calc::Lua::String;//lua string
	using State = FD::Calc::Lua::State;

}

namespace FS::Lua {
	enum class CoreType :uint16_t {
		Integer,
		Number,
		String,
		Function,
		Table,
		Boolean,
		Nil,
	};

	enum class Type : uint16_t {
		Integer,
		Number,
		String,
		Function,
		CppFunctionN,
		Table,
		Userdata,
		LightUserData,
		Bpplean,
		Thread,
		None,
		Nil,
		NoneOrNil
	};

}
