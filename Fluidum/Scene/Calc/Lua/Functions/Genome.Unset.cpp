#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Genome::UNSET> final : public FS::LuaCheck::Common<FS::LuaCheck::CommonType::GENOME> {
public:
	using Common::Common;

public:
	void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 2);

		//第一引数はlua側でチェック

		//pos
		if (types[1] != CoreType::INTEGER) {

			//整数でないなら注意
			if (types[1] == CoreType::NUMBER) {
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_NUMBER_INTEGER> text;
				log->add<FD::LogType::WARNING>(text, funcName, 2);
			}
			else {
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				log->add<FD::LogType::ERROR>(text, funcName, 2, LuaAssist::getCoreTypeName(types[1]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
				throw std::runtime_error("");
			}
		}

	}


};

FS::LuaType::Ret FS::Lua::unset_Genome(State L) {
	LuaCheck::Check<LuaType::Genome::UNSET> check(LuaName::Genome_Unset, L, log, console);
	check.arg();

	const FS::LuaType::Val populationIndex = lua_tointeger(L, 1) - 1;
	const FS::LuaType::Val pos = lua_tointeger(L, 2) - 1;

	assert(genome->genomeType() == FD::Genome::Type::BINARY);

	auto genomes = genome->get<bool>();
	check.popuationSize(populationIndex, genomes->size());
	check.genomeSize(pos, genomes[populationIndex].size());

	{
		auto lock = genome->getLock();
		((*genomes)[populationIndex])[pos] = false;
	}

	return 0;
}



