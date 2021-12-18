#include "../../lua.h"
#include "../Common/array.h"

/*
	1: key
	2: index
	3: nil

	1: key
	2: range[first]
	3: range[last]
*/

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array_erase(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = ::FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 3 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	//check arg type
	if (types[1] != LuAssist::Type::Number) {
		//{}�֐�{}��{}�Ԗڂ̈����̌^�Ɍ�肪����܂��D�n���ꂽ�����̌^: {}�D�����������̌^: {}�D
		Message message(LogType::Type);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "erase", 1, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(LuAssist::Type::Number));
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	const Num indexNum = lua_tonumber(L, 2);
	const Val index = static_cast<Val>(std::round(indexNum)) - 1;

	const std::size_t size = arrayRead->size<T>(key);

	if (index < 0) {
		//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
		Message message(LogType::OutOfRange);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "erase", index, size);
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	//range
	if (static_cast<Val>(size) <= index) {
		//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
		Message message(LogType::OutOfRange);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "erase", index, size);
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	//one element
	if (types[2] == LuAssist::Type::Nil) {
		arrayWrite->erase<T>(key, static_cast<std::size_t>(index));
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//type
	if (types[2] != LuAssist::Type::Number) {
		//{}�֐�{}��{}�Ԗڂ̈����̌^�Ɍ�肪����܂��D�n���ꂽ�����̌^: {}�D�����������̌^: {}�D
		Message message(LogType::Type);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "erase", 2, LuAssist::Utils::typeName(types.at(2)), LuAssist::Utils::typeName(LuAssist::Type::Number));
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	const Num indexNum2 = lua_tonumber(L, 3);
	const Val index2 = static_cast<Val>(std::round(indexNum2)) - 1;

	//range
	if (static_cast<Val>(size) <= index2) {
		//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
		Message message(LogType::OutOfRange);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "erase", index2, size);
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	//elements
	const auto first = static_cast<std::size_t>(index < index2 ? index : index2);
	const auto last = static_cast<std::size_t>(index < index2 ? index2 : index);

	arrayWrite->erase<T>(key, first, last);

	return 0;
}

template
FS::Lua::Ret FS::Lua::Calc::array_erase<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_erase<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array_erase<FD::Calc::Array::ValueType::Bit>(State L);