#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Math::HYPOT> final : public FS::LuaCheck::Common<FS::LuaCheck::CommonType::GENOME> {
public:
	using Common::Common;

public:
	void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 2);//Ç∆ÇËÇ†Ç¶Ç∏ìÒÇ¬Ç‹Ç≈

		//à¯êîÇÃêîÇÕ2 int int
		if (types.size() < 2) {
			if (types.empty()) {
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Math_RandomInt, 1, LuaName::Nil, LuaAssist::getCoreTypeName(LuaType::CoreType::NUMBER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
			else {
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Math_RandomInt, 2, LuaName::Nil, LuaAssist::getCoreTypeName(LuaType::CoreType::NUMBER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
		}

		for (uint16_t i = 0; i < 2; i++) {
			if (types[i] != LuaType::CoreType::INTEGER && types[i] != LuaType::CoreType::NUMBER) {
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Math_RandomInt, i + 1, LuaAssist::getCoreTypeName(types[i]), LuaAssist::getCoreTypeName(LuaType::CoreType::NUMBER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
		}

	}


};

FS::LuaType::Ret FS::Lua::hypot(State L) {
	LuaCheck::Check<LuaType::Math::HYPOT> check(LuaName::Genome_Unset, L, log, console);
	check.arg();

	const LuaType::Num val1 = lua_tonumber(L, 1) - 1;
	const LuaType::Num val2 = lua_tonumber(L, 2) - 1;

	const LuaType::Num distance = std::hypot(val1, val2);
	LuaAssist::pop(L);
	lua_pushnumber(L, distance);
	return 1;
}



