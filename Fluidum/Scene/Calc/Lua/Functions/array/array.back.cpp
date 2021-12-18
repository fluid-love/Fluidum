#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array_back(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = ::FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 2 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	const bool empty = arrayRead->empty<T>(key);

	const bool nil = (types[1] == LuAssist::Type::Nil);
	if (nil) {//return back
		LuAssist::Utils::popAll(L);
		if (empty) {
			//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
			Message message(LogType::OutOfRange);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "back", 1, 0);
			consoleWrite->push(std::move(log));
			throw Internal::Exception();
		}
		else {
			using enum FD::Calc::Array::ValueType;
			const Elm back = arrayRead->back<T>(key);
			Array::pushVal<T>(L, back);
		}
		return 1;
	}

	//check arg type
	{
		constexpr LuAssist::Type luaType = Array::ToLuaType<T>();

		if (types[1] != luaType) {
			//{}関数{}の{}番目の引数の型に誤りがあります．渡された引数の型: {}．正しい引数の型: {}．
			Message message(LogType::Type);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "back", 1, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(luaType));
			consoleWrite->push(std::move(log));
			throw Internal::Exception();
		}
	}

	const Elm val = Array::getVal<T>(L, 2);

	if (empty) {//error
		//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
		Message message(LogType::OutOfRange);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "back", 1, 0);
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	const Elm back = arrayRead->back<T>(key);
	arrayWrite->back<T>(key, val);
	Array::pushVal<T>(L, back);

	return 1;
}

template
FS::Lua::Ret FS::Lua::Calc::array_back<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_back<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_back<FD::Calc::Array::ValueType::Bit>(State L);
