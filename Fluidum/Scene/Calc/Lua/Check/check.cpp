#include "check.h"

//void FS::Lua::Check::numOfArgs(State L, const std::size_t correctSize) {
//
//}


void FS::Lua::Internal::checkNumOfArgs(State L, const std::size_t correctSize, const std::size_t min, const std::size_t max) {
	assert(min != 0);
	assert(max != 0);
	
	const std::size_t size = getArgSize(L);

	//引数の数がおかしい
	if (size != correctSize) {
		//	if (size < min) {
		//		//関数{}の引数の数が少なすぎます．渡された引数の数:{}．正しい引数の数:{}．
		//		Internal::GLog.add<FD::Log::Type::None>();
		//		log->add<FD::LogType::ERROR>(text, funcName, types.size(), 1);
		//		throw std::runtime_error("");
	}
	else {
		////関数{}の引数の数が多すぎます．渡された引数の数:{}．正しい引数の数:{}．
		//LuaText::LuaLog<LuaText::LuaLogType::ARGUMENT_OVER_MAX> text;
		//log->add<FD::LogType::ERROR>(text, funcName, types.size(), 1);
		//throw std::runtime_error("");
	}
}
