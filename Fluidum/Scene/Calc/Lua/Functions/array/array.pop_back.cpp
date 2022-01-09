#include "../../lua.h"

template<FD::Calc::Array::ValueType T>
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_pop_back(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 1 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	const bool empty = arrayRead->empty<T>(key);
	if (empty) {
		//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
		Internal::Message message(Internal::LogType::OutOfRange);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "pop_back", 1, 0);
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	arrayWrite->pop_back<T>(key);

	return 0;
}

template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_pop_back<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_pop_back<FD::Calc::Array::ValueType::String>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array_pop_back<FD::Calc::Array::ValueType::Bit>(State L);
