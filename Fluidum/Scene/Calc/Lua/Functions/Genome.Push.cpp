#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Genome::PUSH> final : public FS::LuaCheck::Common<FS::LuaCheck::CommonType::LOG_CONSOLE> {
public:
	using Common::Common;

public:
	void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 1);

		if (types[0] != CoreType::INTEGER) {
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, LuaName::Genome_Push, 1, LuaAssist::getCoreTypeName(types[0]), "nil");
			throw std::runtime_error("");
		}

	}

};

FS::LuaType::Ret FS::Lua::push_Genome(State L) {
	LuaCheck::Check<LuaType::Genome::PUSH> check(L, log, console);
	check.arg();

	const FS::LuaType::Val key = lua_tointeger(L, 1);

	{
		auto lock = genome->getLock();
		genome->pushBackToData(key);
	}

	return 0;
}



