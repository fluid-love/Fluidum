#include "../../lua.h"

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array_size(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 1 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	const Val size = static_cast<Val>(arrayRead->size<T>(key));

	lua_pushinteger(L, size);

	return 1;
}

template
FS::Lua::Ret FS::Lua::Calc::array_size<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_size<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_size<FD::Calc::Array::ValueType::Bit>(State L);
