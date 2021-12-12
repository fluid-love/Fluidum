#include "../../lua.h"

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array_pop_front(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 1 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	const bool empty = arrayRead->empty<T>(key);
	if (empty) {
		//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
		Message message(LogType::OutOfRange);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "pop_front", 1, 0);
		consoleWrite->add(std::move(log));
		throw Internal::Exception();
	}

	arrayWrite->pop_front<T>(key);

	return 0;
}

template
FS::Lua::Ret FS::Lua::Calc::array_pop_front<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_pop_front<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_pop_front<FD::Calc::Array::ValueType::Bit>(State L);
