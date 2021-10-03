/*
1 populationnum
2 pos
*/

#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Genome::FLIP> final : public FS::LuaCheck::Common<FS::LuaCheck::CommonType::GENOME> {
public:
	using Common::Common;

public:
	void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 2);

		if (types[0] != CoreType::INTEGER && types[0] != CoreType::NUMBER) {
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			auto str = log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
			console->add<FD::LogType::ERROR>(std::move(str));
			throw std::runtime_error("");
		}

		//flip pos
		if (types[1] != CoreType::INTEGER) {
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			auto str = log->add<FD::LogType::ERROR>(text, funcName, 2, LuaAssist::getCoreTypeName(types[1]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
			console->add<FD::LogType::ERROR>(std::move(str));
			throw std::runtime_error("");
		}

	}
};

FS::LuaType::Ret FS::Lua::flip_Genome(State L) {
	LuaCheck::Check<LuaType::Genome::FLIP> check(LuaName::Genome_Flip, L, log,console);
	check.arg();

	const LuaType::Val populationIndex = lua_tointeger(L, 1) - 1;
	const LuaType::Val flipPos = lua_tointeger(L, 2) - 1;

	assert(genome->genomeType() == FD::Genome::Type::BINARY);

	auto genomes = genome->get<bool>();
	check.popuationSize(populationIndex, genomes->size());
	check.genomeSize(flipPos, genomes[populationIndex].size());

	((*genomes)[populationIndex])[flipPos].flip();

	return 0;
}

