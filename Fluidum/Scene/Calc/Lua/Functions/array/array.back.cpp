#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_back(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = ::FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 2 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	const bool empty = arrayRead->empty<T>(key);

	const bool nil = (types[1] == LuAssist::Type::Nil);
	if (nil) {//return back
		LuAssist::Utils::popAll(L);
		if (empty) {
			//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
			Internal::Message message(Internal::LogType::OutOfRange);
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "back", 1, 0);
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
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
			Internal::Message message(Internal::LogType::Type);
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "back", 1, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(luaType));
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
		}
	}

	const Elm val = Array::getVal<T>(L, 2);

	if (empty) {//error
		//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
		Internal::Message message(Internal::LogType::OutOfRange);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "back", 1, 0);
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	const Elm back = arrayRead->back<T>(key);
	arrayWrite->back<T>(key, val);
	Array::pushVal<T>(L, back);

	return 1;
}

template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_back<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_back<FD::Calc::Array::ValueType::String>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_back<FD::Calc::Array::ValueType::Bit>(State L);
