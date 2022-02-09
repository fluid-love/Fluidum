#pragma once

#include "../../lua.h"

namespace FS::Calc::Lua::Array {

	template<FD::Calc::Array::ValueType T>
	auto getVal(State L, const int32_t i) {
		using enum FD::Calc::Array::ValueType;
		if constexpr (T == Number)
			return lua_tonumber(L, i);
		else if constexpr (T == String)
			return lua_tostring(L, i);
		else if constexpr (T == Bit)
			return lua_toboolean(L, i);
		else
			FU::Concept::DelayAssert_N<T>;
	}

	template<FD::Calc::Array::ValueType T>
	auto pushVal(State L, const FD::Calc::Array::ElmType<T>& val) {
		using enum FD::Calc::Array::ValueType;
		if constexpr (T == Number)
			lua_pushnumber(L, val);
		else if constexpr (T == String)
			lua_pushstring(L, val.c_str());
		else if constexpr (T == Bit)
			lua_pushboolean(L, val);
		else
			FU::Concept::DelayAssert_N<T>;
	}

	template<FD::Calc::Array::ValueType T>
	consteval LuAssist::Type ToLuaType() noexcept {
		using namespace FD::Calc::Array;

		if constexpr (T == ValueType::Number)
			return LuAssist::Type::Number;
		else if constexpr (T == ValueType::String)
			return LuAssist::Type::String;
		else if (T == ValueType::Bit)
			return LuAssist::Type::Number;
		else
			FU::Concept::DelayAssert_N<T>;
	}

}