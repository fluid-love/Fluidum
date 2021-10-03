#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Math::RAMDOM_INT> final : private Common<CommonType::LOG_CONSOLE> {
public:
	using Common::Common;

public:
	void arg() {
		const auto types = LuaAssist::getCoreTypes(L);

		//ˆø”‚Ì”‚Í2 int int
		if (types.size() < 2) {
			if (types.empty()) {
				//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Math_RandomInt, 1, LuaName::Nil , LuaAssist::getCoreTypeName(LuaType::CoreType::INTEGER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
			else {
				//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Math_RandomInt, 2, LuaName::Nil , LuaAssist::getCoreTypeName(LuaType::CoreType::INTEGER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
		}

		for (uint16_t i = 0; i < 2; i++) {
			if (types[i] != LuaType::CoreType::INTEGER) {
				//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÉŒë‚Á‚½Œ^‚ª“n‚³‚ê‚Ü‚µ‚½D“n‚³‚ê‚½ˆø”‚ÌŒ^:{}D³‚µ‚¢Œ^:{}D
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Math_RandomInt, i + 1, LuaAssist::getCoreTypeName(types[i]), LuaAssist::getCoreTypeName(LuaType::CoreType::INTEGER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
		}
	}

};

FS::LuaType::Ret FS::Lua::randomInt(State L) {
	LuaCheck::Check<LuaType::Math::RAMDOM_INT> check(L, log, console);

	//Œ^ƒ`ƒFƒbƒN
	check.arg();

	const LuaType::Val min = lua_tointeger(L, 1);
	const LuaType::Val max = lua_tointeger(L, 2);

	LuaAssist::pop(L);

	std::random_device randDevice;
	std::mt19937 engine(randDevice());
	std::uniform_int_distribution<LuaType::Val> rand(min, max);

	lua_pushinteger(L, rand(engine));
	return 1;
}