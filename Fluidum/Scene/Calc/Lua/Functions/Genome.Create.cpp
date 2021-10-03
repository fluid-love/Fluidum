#include "../FSlua.h"

using namespace FS::LuaType;

template<>
class FS::LuaCheck::Check<Genome::CREATE> final : private Common<CommonType::LOG_CONSOLE> {
public:
	using Common::Common;

private:
	const char* const funcName = "Genome.Create";
public:
	_NODISCARD std::array<bool, 4> arg() const {
		const auto types = LuaAssist::getCoreTypes(L);

		assert(types.size() == 4 && "Genome.Create ArgSize Over.");

		//arg size == 4 �͊m��
		std::array<bool, 4> result = { true,true,true,true };

		for (uint16_t i = 0; i < 2; i++) {
			if (types[i] != CoreType::INTEGER && types[i] != CoreType::NUMBER) {
				if (types[i] == CoreType::NIL) {
					result[i] = false;
				}
				else {
					//�֐�{}��{}�Ԗڂ̈����Ɍ�����^���n����܂����D�n���ꂽ�����̌^:{}�D�������^:{}�D
					LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
					std::string str = log->add<FD::LogType::ERROR>(text, funcName, i + 1, LuaAssist::getCoreTypeName(types[i]), LuaAssist::getCoreTypeName(CoreType::INTEGER));
					console->add<FD::LogType::ERROR>(std::move(str));
					throw std::runtime_error("");
				}
			}
		}

		for (uint16_t i = 2; i < 4; i++) {
			if (types[i] != CoreType::STRING) {
				if (types[i] == CoreType::NIL) {
					result[i] = false;
				}
				else {
					//�֐�{}��{}�Ԗڂ̈����Ɍ�����^���n����܂����D�n���ꂽ�����̌^:{}�D�������^:{}�D
					LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
					std::string str = log->add<FD::LogType::ERROR>(text, funcName, i + 1, LuaAssist::getCoreTypeName(types[i]), LuaAssist::getCoreTypeName(CoreType::STRING));
					console->add<FD::LogType::ERROR>(std::move(str));
					throw std::runtime_error("");
				}
			}
		}

		return result;
	}

	void val(const FD::Genome::Property* const prop, const Val populationSize, const Val genomeSize) {

		if (populationSize > prop->limit.populationSizeMax) {
			//�֐�{}��{}�Ԗڂ̈����ɓn���ꂽ�l���C�������ꂽ����l�𒴂��Ă��܂��D�n���ꂽ�l:{}�D����l{}�D
			LuaText::LuaLog<LuaText::LuaLogType::OVER_LIMIT_MAX> text;
			auto str = log->add<FD::LogType::ERROR>(text, funcName, 1, populationSize, prop->limit.populationSizeMax);
			console->add<FD::LogType::ERROR>(std::move(str));
			throw std::runtime_error("");
		}
		else if (populationSize < prop->limit.populationSizeMin) {
			//�֐�{}��{}�Ԗڂ̈����ɓn���ꂽ�l���C�������ꂽ�����l�𒴂��Ă��܂��D�n���ꂽ�l:{}�D�����l{}�D
			LuaText::LuaLog<LuaText::LuaLogType::OVER_LIMIT_MIN> text;
			auto str = log->add<FD::LogType::ERROR>(text, funcName, 1, populationSize, prop->limit.populationSizeMin);
			console->add<FD::LogType::ERROR>(std::move(str));
			throw std::runtime_error("");
		}


		if (genomeSize > prop->limit.genomeSizeMax) {
			//�֐�{}��{}�Ԗڂ̈����ɓn���ꂽ�l���C�������ꂽ����l�𒴂��Ă��܂��D�n���ꂽ�l:{}�D����l{}�D
			LuaText::LuaLog<LuaText::LuaLogType::OVER_LIMIT_MAX> text;
			auto str = log->add<FD::LogType::ERROR>(text, funcName, 2, genomeSize, prop->limit.genomeSizeMax);
			console->add<FD::LogType::ERROR>(std::move(str));
			throw std::runtime_error("");
		}
		else if (genomeSize < prop->limit.genomeSizeMin) {
			//�֐�{}��{}�Ԗڂ̈����ɓn���ꂽ�l���C�������ꂽ�����l�𒴂��Ă��܂��D�n���ꂽ�l:{}�D�����l{}�D
			LuaText::LuaLog<LuaText::LuaLogType::OVER_LIMIT_MIN> text;
			auto str = log->add<FD::LogType::ERROR>(text, funcName, 2, populationSize, prop->limit.genomeSizeMin);
			console->add<FD::LogType::ERROR>(std::move(str));
			throw std::runtime_error("");
		}


	}

};

FD::Genome::Type toGenomeType(const std::string type) noexcept {
	if (type == "Bin" || type == "bin" || type == "Binary" || type == "binary")
		return FD::Genome::Type::BINARY;
	if (type == "Dbl" || type == "dbl" || type == "Double" || type == "double")
		return FD::Genome::Type::DOUBLE;

	return FD::Genome::Type::NONE;
}

template<typename T>
void initGenome(FD::Genome::Genomes<T>* genomes, String const initType, const Val populationSize, const Val genomeSize) {
	const std::string initTypeStr(initType);
	
	if (initTypeStr.empty())
		return;

	genomes->resize(populationSize);

	//�S�Ẳ���ɑ΂��ă����_���Ȓl������
	if (initTypeStr == "Random" || initTypeStr == "random") {
		for (auto&& x : *genomes) {
			x.resize(genomeSize);
			x.random(x.begin(), x.end());
		}
	}


}

Ret FS::Lua::create_Genome(State L) {
	//��d�쐬���֎~
	if (genome->genomeType() != FD::Genome::Type::NONE) {
		LuaText::LuaLog<LuaText::LuaLogType::ALREADY_CREATED> text;
		//�֐�{}���Ă΂�܂������C���ɍ쐬����Ă��܂��D
		log->add<FD::LogType::ERROR>(text, "Genome.Create");
		throw std::runtime_error("");
	}

	LuaCheck::Check<LuaType::Genome::CREATE> check(L, log, console);
	const auto valid = check.arg();

	Val populationSize = 0; //�����l
	Val genomeSize = 0;     //�����l

	//�L���Ȓl������Έ��������Ă���
	if (valid[0]) {
		populationSize = lua_tointeger(L, 1);
	}
	if (valid[1]) {
		genomeSize = lua_tointeger(L, 2);
	}

	String genomeTypeStr = "Bin";
	String initTypeStr = "";

	Scene::createData<FD::Genome::Property>();//property��ǂݎ�邽��
	const auto prop = Scene::getData<FD::Genome::Property>();

	check.val(prop, populationSize, genomeSize);

	if (valid[2]) {
		genomeTypeStr = lua_tostring(L, 3);
	}
	if (valid[3]) {
		initTypeStr = lua_tostring(L, 4);
	}

	//�����񂩂�genome type�����
	const FD::Genome::Type genomeType = toGenomeType(genomeTypeStr);

	//genome type��������
	genome->initType(genomeType);

	//������	
	//genome->call<FluidumData_Genome_CallFunctions(initGenome)>(initTypeStr, populationSize, genomeSize);

	return 0;
}