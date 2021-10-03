#pragma once

#include "../../Common/common.h"
#include "../../Libraries/lua-5.4.3/src/lua.hpp"

namespace FS::Lua {

	using Ret = int;
	using Num = LUA_NUMBER;//lua number
	using Val = long long;
	using String = const char*;//lua string
	using State = lua_State*;

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

namespace FS::Lua::Function {

	enum class Genome : uint16_t {
		Create,
		SinglePointCrossover,
		Flip,
		Set,
		Unset,
		Erase,
		ToUint,
		Push,
	};

	enum class Math : uint16_t {
		RamdomInt,
		Hypot
	};

	enum class Plot : uint16_t {
		Create,
		Push,
		Marker,
	};

	enum class Draw : uint16_t {
		Create,
		UpdateVertex,
		UpdateIndex,
		Clear
	};

	enum class System : uint16_t {
		SleepSeconds,
		SleepMilliSeconds,
		Terminate
	};


	template<auto T>
	concept IsFunction =
		std::same_as<decltype(T), Genome> ||
		std::same_as<decltype(T), Math> ||
		std::same_as<decltype(T), Plot> ||
		std::same_as<decltype(T), Draw> ||
		std::same_as<decltype(T), System>;



}