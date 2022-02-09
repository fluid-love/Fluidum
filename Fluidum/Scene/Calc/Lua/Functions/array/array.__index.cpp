#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___index(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = ::FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 2 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	//check arg type
	if (types[1] != LuAssist::Type::Number) {
		//{}�֐�{}��{}�Ԗڂ̈����̌^�Ɍ�肪����܂��D�n���ꂽ�����̌^: {}�D�����������̌^: {}�D
		Internal::Message message(Internal::LogType::Type);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__index", 1, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(LuAssist::Type::Number));
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	//get index
	const Num val = lua_tonumber(L, 2);
	const Val index = static_cast<Val>(std::round(val)) - 1;

	if (index < 0) {
		//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
		Internal::Message message(Internal::LogType::OutOfRange);
		const Size arraySize = arrayRead->size<T>(key);
		Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__index", index, arraySize);
		consoleWrite->push(Internal::GMessenger.getMessage());
		Internal::Exception::throwException();
	}

	Elm ret;
	try {
		ret = arrayRead->at<T>(key, static_cast<Size>(index));
	}
	catch (const FD::Calc::ArrayRead::Exception val) {
		using enum FD::Calc::ArrayRead::Exception;
		if (val == Range) {
			//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
			Internal::Message message(Internal::LogType::OutOfRange);
			const Size arraySize = arrayRead->size<T>(key);
			Internal::GMessenger.add<FU::Log::Type::None>(__FILE__, __LINE__, message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__index", index, arraySize);
			consoleWrite->push(Internal::GMessenger.getMessage());
			Internal::Exception::throwException();
		}
		else {
			FluidumScene_Log_InternalWarning_Enum(val);
		}

		FluidumScene_Log_InternalWarning();
		Internal::Exception::throwInternalError();
	}

	Array::pushVal<T>(L, ret);

	return 1;
}

template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___index<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___index<FD::Calc::Array::ValueType::String>(State L);
template
FS::Calc::Lua::Ret FS::Calc::Lua::Run::array___index<FD::Calc::Array::ValueType::Bit>(State L);
