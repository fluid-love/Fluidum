/*
1 index of population 1 固定->lua側でチェックする
2 index of population 2 引数
3 crossover position    引数
*/

#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Genome::SINGLE_POINT_CROSSOVER> final : private Common<FS::LuaCheck::CommonType::LOG_CONSOLE> {
public:
	using Common::Common;
public:
	_NODISCARD void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 3);

		for (uint16_t i = 1; i < 3; i++) {
			if (types[i] != CoreType::INTEGER && types[i] != CoreType::NUMBER) {
				//関数{}の{}番目の引数に誤った型が渡されました．渡された引数の型:{}．正しい型:{}．
				LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
				auto str = log->add<FD::LogType::ERROR>(text, LuaName::Genome_SinglePointCrossOver, i, LuaAssist::getCoreTypeName(types[i]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
				console->add<FD::LogType::ERROR>(std::move(str));
				throw std::runtime_error("");
			}
		}

	}

	template<typename T>
	void val(const FD::Genome::Genomes<T>* const genomes, const Val pop1Index, const Val pop2Index, const Val crossOverPos) {

		const auto popSize = genomes->size();

		if (static_cast<decltype(popSize)>(pop1Index) >= popSize) {
			this->throwPopSize(popSize, pop1Index);
		}
		if (static_cast<decltype(popSize)>(pop2Index) >= popSize) {
			this->throwPopSize(popSize, pop2Index);
		}

		const auto genomeSize1 = (*genomes)[pop1Index].size();
		const auto genomeSize2 = (*genomes)[pop2Index].size();

		if (static_cast<decltype(genomeSize1)>(crossOverPos) >= genomeSize1) {
			this->throwGenomeSize(genomeSize1, crossOverPos);
		}
		if (static_cast<decltype(genomeSize2)>(crossOverPos) >= genomeSize2) {
			this->throwGenomeSize(genomeSize2, crossOverPos);
		}
	}

private:
	[[noreturn]] void throwPopSize(const Val popSize, const Val popIndex) {
		LuaText::GenomeLog<LuaText::GenomeLogType::INDEX_OVER_MAX_POPULATION> text;
		auto str = log->add<FD::LogType::ERROR>(text, LuaName::Genome_SinglePointCrossOver, popIndex + 1, popSize);
		console->add<FD::LogType::ERROR>(std::move(str));
		throw std::runtime_error("");
	}

	[[noreturn]] void throwGenomeSize(const Val genomeSize, const Val crossOverPos) {
		LuaText::GenomeLog<LuaText::GenomeLogType::INDEX_OVER_MAX_GENOME> text;
		auto str = log->add<FD::LogType::ERROR>(text, LuaName::Genome_SinglePointCrossOver, crossOverPos, genomeSize);
		console->add<FD::LogType::ERROR>(std::move(str));
		throw std::runtime_error("");
	}


};

template<typename T>
void crossOver(FD::Genome::Genomes<T>* genomes, const FS::LuaType::Val parent1Index, const FS::LuaType::Val parent2Index, const FS::LuaType::Val crossOverPos) {
	//posの右側をスワップ
	CppGA::singlePointCrossOver((*genomes)[parent1Index], (*genomes)[parent2Index], crossOverPos);
}

FS::LuaType::Ret FS::Lua::singlePointCrossOver(State L) {
	using CheckType = LuaCheck::Check<LuaType::Genome::SINGLE_POINT_CROSSOVER>;
	CheckType check(L, log, console);
	check.arg();

	const LuaType::Val parent1Index = lua_tointeger(L, 1) - 1;
	const LuaType::Val parent2Index = lua_tointeger(L, 2) - 1;

	const LuaType::Val crossOverPos = lua_tointeger(L, 3) - 1;

	//読み取りしかしないのでlockは必要ない
	genome->call<FluidumData_Genome_CallFunctions(&CheckType::val)>(&check, parent1Index, parent2Index, crossOverPos);

	{
		auto lock = genome->getLock();
		genome->call<FluidumData_Genome_CallFunctions(&CheckType::val)>(&check, parent1Index, parent2Index, crossOverPos);
	}

	return 0;
}

