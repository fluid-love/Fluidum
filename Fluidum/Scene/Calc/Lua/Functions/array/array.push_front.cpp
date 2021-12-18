#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array_push_front(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	bool table = false;

	//table
	if (types.size() == 2) {

		if (types[1] == LuAssist::Type::Table) {
			table = true;
		}
		else if (types[1] == Array::ToLuaType<T>()) {
			;
		}
		else {
			//{}ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½ˆø”‚ÌŒ^: {}D³‚µ‚¢ˆø”‚ÌŒ^: {}D
			Message message(LogType::Type);
			std::string type = LuAssist::Utils::typeName(LuAssist::Type::Table);
			(type += ',') += LuAssist::Utils::typeName(Array::ToLuaType<T>());	
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "push_front", 1, LuAssist::Utils::typeName(types.at(1)), type);
			consoleWrite->push(std::move(log));
			throw Internal::Exception();
		}
	}

	std::vector<Elm> vals{};
	if (table) {
		using enum  FD::Calc::Array::ValueType;
		if constexpr (T == Number)
			vals = LuAssist::Utils::Table::convertTableToArrayOfNumbers(L, 2);
		else if constexpr (T == String)
			vals = LuAssist::Utils::Table::convertTableToArrayOfStrings(L, 2);
		else
			vals = LuAssist::Utils::Table::convertTableToArrayOfBooleans(L, 2);

		arrayWrite->push_front<T>(key, std::move(vals));
		return 0;
	}


	//1    2    3    4    ...
	//key, val, val, val, ...
	vals.reserve(types.size() - 1);
	int32_t count = 2;
	while (lua_isnone(L, count)) {
		if (types[count - 1] != Array::ToLuaType<T>()) {
			//{}ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½ˆø”‚ÌŒ^: {}D³‚µ‚¢ˆø”‚ÌŒ^: {}D
			Message message(LogType::Type);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "push_front", count - 1, LuAssist::Utils::typeName(types[count - 1]), LuAssist::Utils::typeName(Array::ToLuaType<T>()));
			consoleWrite->push(std::move(log));
			throw Internal::Exception();
		}

		vals.emplace_back(Array::getVal<T>(L, count));

		count++;
	}

	arrayWrite->push_front<T>(key, std::move(vals));

	return 0;
}

template
FS::Lua::Ret FS::Lua::Calc::array_push_front<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_push_front<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_push_front<FD::Calc::Array::ValueType::Bit>(State L);