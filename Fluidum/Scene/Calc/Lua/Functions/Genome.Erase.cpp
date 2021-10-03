/*
�Ƃ肠�����e�[�u���̂�
*/

#include "../FSlua.h"

template<>
class FS::LuaCheck::Check<FS::LuaType::Genome::ERASE> final : public FS::LuaCheck::Common<FS::LuaCheck::CommonType::GENOME> {
public:
	using Common::Common;

public:
	void arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 1);

		if (types[0] != CoreType::TABLE) {
			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
			log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::TABLE));
			throw std::runtime_error("");
		}

		//if (types[0] != CoreType::INTEGER && types[0] != CoreType::NUMBER) {
		//	//�֐�{}��{}�Ԗڂ̈����Ɍ�����^���n����܂����D�n���ꂽ�����̌^:{}�D�������^:{}�D
		//	LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
		//	log->add<FD::LogType::ERROR>(text, funcName, 1, LuaAssist::getCoreTypeName(types[0]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
		//	throw std::runtime_error("");
		//}

	}

	template<typename T>
	void eraseRange(const FD::Genome::Genomes<T>* const genomes, const std::vector<FD::Genome::PopulationSize>& indices) {
		const auto size = genomes->size();
		for (auto& x : indices) {
			this->popuationSize(x, size);
		}
	}
};

template<typename T>
void eraseGenome(FD::Genome::Genomes<T>* genomes, const std::vector<FD::Genome::PopulationSize>& indices) {
	for (const auto& x : indices)
		genomes->erase(genomes->begin() + x);
}

FS::LuaType::Ret FS::Lua::erase_Genome(State L) {
	using CheckType = LuaCheck::Check<LuaType::Genome::ERASE>;
	CheckType check(LuaName::Genome_Erase, L, log, console);
	check.arg();

	//���������-1�����
	std::vector<FD::Genome::PopulationSize> eraseIndices = LuaAssist::toVector<FD::Genome::PopulationSize>(L);

	//erase indices��population size�𒴂��Ă��Ȃ��� 
	//genome->call<FluidumData_Genome_CallFunctions(&CheckType::eraseRange)>(&check, eraseIndices);

	//�~���Ƀ\�[�g
	std::sort(eraseIndices.begin(), eraseIndices.end(), [](int left, int right) { return left > right; });

	//����
	//genome->call<FluidumData_Genome_CallFunctions(eraseGenome)>(eraseIndices);

	return 0;
}

