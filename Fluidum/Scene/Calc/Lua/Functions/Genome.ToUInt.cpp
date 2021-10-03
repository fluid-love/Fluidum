/*
1 target
2 size
3 pos
*/

#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Genome::TO_UINT> final : public FS::LuaCheck::Common<FS::LuaCheck::CommonType::GENOME> {
public:
	using Common::Common;

public:
	void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 3);

		//第一引数はlua側でチェック

		//pos
		for (uint16_t i = 1; i < 3; i++) {
			if (types[i] != CoreType::INTEGER) {

				//整数でないなら注意
				if (types[i] == CoreType::NUMBER) {
					LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_NUMBER_INTEGER> text;
					log->add<FD::LogType::WARNING>(text, funcName, i + 1);
				}
				else {
					LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
					log->add<FD::LogType::ERROR>(text, funcName, i + 1, LuaAssist::getCoreTypeName(types[1]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
					throw std::runtime_error("");
				}
			}
		}
	}


};

FS::LuaType::Ret FS::Lua::toUInt_Genome(State L) {
	LuaCheck::Check<LuaType::Genome::TO_UINT> check(LuaName::Genome_ToUInt, L, log, console);
	check.arg();

	const FS::LuaType::Val populationIndex = lua_tointeger(L, 1) - 1;
	const FS::LuaType::Val size = lua_tointeger(L, 2);
	const FS::LuaType::Val pos = lua_tointeger(L, 3) - 1;

	assert(genome->genomeType() == FD::Genome::Type::BINARY);

	const FD::Genome::Genomes<FD::Genome::BinType>* const genomes = genome->get<bool>();
	check.popuationSize(populationIndex, genomes->size());
	check.genomeSize(pos + size, genomes[populationIndex].size());

	//読み取りのみ
	std::string binary;
	binary.resize(size);
	std::fill(binary.begin(), binary.end(), '0');

	for (std::size_t i = pos; i < (pos + size); i++) {
		if ((*genomes)[populationIndex][i]) {
			binary[i] = '1';
		}
		i++;
	}

	const auto result = std::strtoull(binary.c_str(), nullptr, 2);

	LuaAssist::pop(L);
	lua_pushinteger(L, result);
	return 1;
}



