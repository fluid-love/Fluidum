#include "../../lua.h"

namespace FS::Calc::Lua {

	constexpr const char* ArrayValueNames[] = {
		"number","Number","num","Num",
		"string","String","str","Str",
		"bit","Bit","binary","Binary",
	};

	[[nodiscard]] std::string concatArrayValueNames() {
		std::string ret{};
		for (auto x : ArrayValueNames) {
			(ret += x) += ',';
		}
		return ret;
	}

	[[nodiscard]] std::optional<FD::Calc::Array::ValueType> stringToArrayValueType(const std::string& arg) {

		using enum FD::Calc::Array::ValueType;
		constexpr FD::Calc::Array::ValueType types[] = {
			Number,
			String,
			Bit
		};

		for (UIF16 i = 0, j = 0; i < std::extent_v<decltype(ArrayValueNames), 0>; i++) {
			if (arg == ArrayValueNames[i])
				return types[i];

			if ((i + 1) % 4 == 0)
				j++;
		}

		return std::nullopt;
	}

}

FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_make(State L) {

	const bool argTypes = LuAssist::Utils::checkArgTypes<LuAssist::Type::String>(L);
	if (!argTypes) {
		//{}ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½ˆø”‚ÌŒ^: {}D³‚µ‚¢ˆø”‚ÌŒ^: {}D
		Internal::Message message(Internal::LogType::Type);
		const auto types = LuAssist::Utils::types(L);
		assert(types.size() == 1);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "make", 1, LuAssist::Utils::typeName(types.at(0)), LuAssist::Utils::typeName(LuAssist::Type::String));
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	const String str = lua_tostring(L, 1);
	const auto type = stringToArrayValueType(str);

	if (!type.has_value()) {//not match
		//{}ŠÖ”{}‚Ìˆø”‚É–³Œø‚È•¶š—ñ‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½•¶š—ñ{}D³‚µ‚¢•¶š—ñ{}D
		Internal::Message message(Internal::LogType::NotMatch);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), str, concatArrayValueNames());
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	FD::Calc::Array::Key key;

	try {
		key = arrayWrite->make(type.value());
	}
	catch (const FD::Calc::ArrayWrite::Exception val) {
		using enum FD::Calc::ArrayWrite::Exception;
		if (val == MaxSize) {
			//{}ŠÖ”{}‚ªŒÄ‚Î‚ê‚Ü‚µ‚½‚ªì¬‚Å‚«‚é{}‚ÌÅ‘å”‚ğ’´‚¦‚Ä‚¢‚Ü‚·DÅ‘å’l{}D
			Internal::Message message(Internal::LogType::NotMatch);
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "make", FD::Calc::Array::Limits::MaxSize);
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
		}
		else {
			FluidumScene_Log_InternalWarning_Enum(val);
		}
		FluidumScene_Console_InternalError();
		Internal::Exception::throwInternalError();
	}

	lua_pushinteger(L, key);

	{
		using enum FD::Calc::Array::ValueType;
		if (type == Number)
			lua_pushstring(L, "n");
		else if (type == String)
			lua_pushstring(L, "s");
		else
			lua_pushstring(L, "b");
	}
	return 2;
}
