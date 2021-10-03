//#include "../lua.h"
//
//
//void arg() {
//	const auto types = LuaAssist::getCoreTypes(L);
//
//
//
//	if (types[0] != CoreType::INTEGER && types[0] != CoreType::NUMBER) {
//		//�֐�{}��{}�Ԗڂ̈����Ɍ�����^���n����܂����D�n���ꂽ�����̌^:{}�D�������^:{}�D
//		LuaText::LuaLog<LuaText::LuaLogType::WRONG_TYPE_ARGUMENT> text;
//		log->add<FD::LogType::ERROR>(
//			text,
//			funcName,
//			0,
//			LuaAssist::getCoreTypeName(types[0]),
//			std::string(LuaAssist::getCoreTypeName(CoreType::INTEGER)) + LuaAssist::getCoreTypeName(CoreType::NUMBER)
//			);
//		throw std::runtime_error("");
//	}
//}
//
//template <typename T>
//auto seconds_to_duration(T seconds) {
//	return std::chrono::duration<T, std::ratio<1>>(seconds);
//}
//
//FS::Lua::Ret FS::LuaCalc::sleepSeconds(Lua::State L) {
//	using namespace Lua;
//	const Num sleepTimeSeconds = lua_tonumber(L, 1);
//	auto time = std::chrono::duration<Num>(sleepTimeSeconds);
//
//	std::this_thread::sleep_for(time);
//
//	LuaAssist::pop(L);
//	return 0;
//}
