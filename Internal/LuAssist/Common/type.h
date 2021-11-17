#pragma once

#include <lua.hpp>
#include <type_traits>
#include <vector>
#include <string>
#include <format>
#include <cassert>
#include <optional>

namespace LuAssist {
	
	using Ret = int;//return
	using Num = LUA_NUMBER;//double
	using Val = LUA_INTEGER;//long long
	using String = const char*;//lua string
	using State = lua_State*;//CFunction arg type

}

namespace LuAssist {
	enum class BasicType :uint16_t {
		Number,
		String,
		Nil,
		Boolean,
		Table,
		Function,
		UserData,
		Thread,
	};

	enum class Type : uint16_t {
		Integer,
		Number,
		String,
		Nil,
		Boolean,
		Table,
		CFunction,
		Function,
		LightUserData,
		UserData,
		Thread,
		
		
		None,
		//NoneOrNil
	};

}
