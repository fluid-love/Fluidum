#include "../../lua.h"

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array_clear(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 1 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	arrayWrite->clear<T>(key);

	return 0;
}

template
FS::Lua::Ret FS::Lua::Calc::array_clear<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_clear<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_clear<FD::Calc::Array::ValueType::Bit>(State L);
