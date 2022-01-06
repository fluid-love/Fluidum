#include "../../lua.h"

template<FD::Calc::Array::ValueType T>
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___gc(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 1 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalWarning();
		Internal::throwInternalError();
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	arrayWrite->reusable<T>(key);

	return 0;
}

template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___gc<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___gc<FD::Calc::Array::ValueType::String>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___gc<FD::Calc::Array::ValueType::Bit>(State L);
