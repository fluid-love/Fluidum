#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_set(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
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
			Internal::Message message(Internal::LogType::Type);
			std::string type = LuAssist::Utils::typeName(LuAssist::Type::Table);
			(type += ',') += LuAssist::Utils::typeName(Array::ToLuaType<T>());	
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "set", 1, LuAssist::Utils::typeName(types.at(1)), type);
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
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

		arrayWrite->set<T>(key, std::move(vals));
		return 0;
	}


	//1    2    3    4    ...
	//key, val, val, val, ...
	vals.reserve(types.size() - 1);
	int32_t count = 2;
	while (lua_isnone(L, count)) {
		if (types[count - 1] != Array::ToLuaType<T>()) {
			//{}ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½ˆø”‚ÌŒ^: {}D³‚µ‚¢ˆø”‚ÌŒ^: {}D
			Internal::Message message(Internal::LogType::Type);
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "set", count - 1, LuAssist::Utils::typeName(types[count - 1]), LuAssist::Utils::typeName(Array::ToLuaType<T>()));
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
		}

		vals.emplace_back(Array::getVal<T>(L, count));

		count++;
	}

	arrayWrite->set<T>(key, std::move(vals));

	return 0;

}

template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_set<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_set<FD::Calc::Array::ValueType::String>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_set<FD::Calc::Array::ValueType::Bit>(State L);
