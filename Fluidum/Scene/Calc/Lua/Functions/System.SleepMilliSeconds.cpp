//#include "../lua.h"
//
//using namespace FS::LuaType;
//
//template<>
//class FS::LuaCheck::Check<System::SLEEP_MILLISECONDS> final : private Common<> {
//public:
//	using Common::Common;
//
//private:
//	const char* const funcName = "System.SleepMilliSeconds";
//
//public:
//	void arg() {
//		const auto types = LuaAssist::getCoreTypes(L);
//
//		//�����̐�����������
//		if (types.size() != 1) {
//			if (types.size() < 1) {
//				//�֐�{}�̈����̐������Ȃ����܂��D�n���ꂽ�����̐�:{}�D�����������̐�:{}�D
//				LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MIN> text;
//				log->add<FD::LogType::ERROR>(text, funcName, types.size(), 1);
//				throw std::runtime_error("");
//			}
//			else {
//				//�֐�{}�̈����̐����������܂��D�n���ꂽ�����̐�:{}�D�����������̐�:{}�D
//				LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
//				log->add<FD::LogType::ERROR>(text, funcName, types.size(), 1);
//				throw std::runtime_error("");
//			}
//		}
//
//		if (types[0] != CoreType::INTEGER) {
//			//�֐�{}��{}�Ԗڂ̈����Ɍ�����^���n����܂����D�n���ꂽ�����̌^:{}�D�������^:{}�D
//			LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
//			log->add<FD::LogType::WARNING>(
//				text,
//				funcName,
//				0,
//				LuaAssist::getCoreTypeName(types[0]),
//				std::string(LuaAssist::getCoreTypeName(CoreType::INTEGER)) + LuaAssist::getCoreTypeName(CoreType::NUMBER)
//				);
//
//			//������infolog
//		}
//
//	}
//};
//
//Ret FS::Lua::sleepMilliSeconds(State L) {
//	LuaCheck::Check<LuaType::System::SLEEP_MILLISECONDS> check(L, log);
//	check.arg();
//
//	const Val sleepTimeMilliSeconds = lua_tonumber(L, 1);
//
//	std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMilliSeconds));
//
//	LuaAssist::pop(L);
//	return 0;
//}
