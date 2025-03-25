#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___newindex(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = ::FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 3 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	//check arg type
	if (types[1] != LuAssist::Type::Number) {
		//{}関数{}の{}番目の引数の型に誤りがあります．渡された引数の型: {}．正しい引数の型: {}．
		Internal::Message message(Internal::LogType::Type);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__newindex", 1, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(LuAssist::Type::Number));
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}
	constexpr LuAssist::Type valType = Array::ToLuaType<T>();
	if (types[2] != valType) {
		//{}関数{}の{}番目の引数の型に誤りがあります．渡された引数の型: {}．正しい引数の型: {}．
		Internal::Message message(Internal::LogType::Type);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__newindex", 2, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(valType));
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	const Num indexNum = lua_tonumber(L, 2);
	const Val index = static_cast<Val>(std::round(indexNum)) - 1;

	if (index < 0) {
		//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
		Internal::Message message(Internal::LogType::OutOfRange);
		const Size arraySize = arrayRead->size<T>(key);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__newindex", index, arraySize);
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	const Elm val = Array::getVal<T>(L, 3);

	try {
		arrayWrite->at<T>(key, static_cast<std::size_t>(index), val);
	}
	catch (const FD::Calc::ArrayWrite::Exception val) {
		using enum FD::Calc::ArrayWrite::Exception;
		if (val == Range) {
			//{}関数{}によって{}番目の要素にアクセスしようとしましたが範囲外でした．要素のサイズ{}．
			Internal::Message message(Internal::LogType::OutOfRange);
			const Size arraySize = arrayRead->size<T>(key);
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__newindex", index, arraySize);
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
		}
		else {
			FluidumScene_Log_InternalWarning_Enum(val);
		}

		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
	}

	return 0;
}

template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___newindex<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___newindex<FD::Calc::Array::ValueType::String>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___newindex<FD::Calc::Array::ValueType::Bit>(State L);
