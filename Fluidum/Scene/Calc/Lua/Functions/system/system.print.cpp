#include "../../lua.h"

FS::Calc::Lua::Ret FS::Calc::Lua::Run::system_print(State L) {
	using enum LuAssist::Type;

	const auto types = LuAssist::Utils::types(L);
	assert(types.size() < std::numeric_limits<int>::max());

	std::string str{};
	
	for (int i = 1; auto & type : types) {
		if (type == Integer) {
			const Val integer = lua_tointeger(L, i);
			str += std::format("{}",integer);
		}
		else if (type == Number) {
			const Num num = lua_tonumber(L, i);
			str += std::format("{}", num);
		}
		else if (type == String) {
			LuAssist::String string = lua_tostring(L, i);
			str += string;
		}
		else if (type == Nil) {
			str += "nil";
		}
		else if (type == Boolean) {
			const bool val = lua_toboolean(L, i);
			str += val ? "true" : "false";
		}
		else if (type == Table) {
			str += "table";
		}
		else if (type == CFunction) {
			str += "cfunction";
		}
		else if (type == Function) {
			str += "function";
		}
		else if (type == LightUserData) {
			str += "lightuserdata";
		}
		else if (type == UserData) {
			str += "userdata";
		}
		else if (type == Thread) {
			str += "thread";
		}
		else {
			FluidumScene_Log_InternalWarning();
		}
		
		str += " ";
		i++;
	}

	consoleWrite->push(std::move(str));

	return 0;
}
