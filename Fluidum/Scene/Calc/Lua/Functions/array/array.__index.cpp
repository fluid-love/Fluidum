#include "../../lua.h"
#include "../Common/array.h"

template<FD::Calc::Array::ValueType T>
FS::Lua::Ret FS::Lua::Calc::array___index(State L) {
	static_assert(FD::Calc::Array::IsValueType<T>);

	using Elm = ::FD::Calc::Array::ElmType<T>;

	const auto types = LuAssist::Utils::types(L);
	if (types.size() != 2 && types[0] == LuAssist::Type::Integer) {
		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	const FD::Calc::Array::Key key = static_cast<FD::Calc::Array::Key>(lua_tointeger(L, 1));

	//check arg type
	if (types[1] != LuAssist::Type::Number) {
		//{}�֐�{}��{}�Ԗڂ̈����̌^�Ɍ�肪����܂��D�n���ꂽ�����̌^: {}�D�����������̌^: {}�D
		Message message(LogType::Type);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__index", 1, LuAssist::Utils::typeName(types.at(1)), LuAssist::Utils::typeName(LuAssist::Type::Number));
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	//get index
	const Num val = lua_tonumber(L, 2);
	const Val index = static_cast<Val>(std::round(val)) - 1;

	if (index < 0) {
		//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
		Message message(LogType::OutOfRange);
		const std::size_t arraySize = arrayRead->size<T>(key);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__index", index, arraySize);
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	Elm ret;
	try {
		ret = arrayRead->at<T>(key, static_cast<std::size_t>(index));
	}
	catch (const FD::Calc::ArrayRead::Exception val) {
		using enum FD::Calc::ArrayRead::Exception;
		if (val == Range) {
			//{}�֐�{}�ɂ����{}�Ԗڂ̗v�f�ɃA�N�Z�X���悤�Ƃ��܂������͈͊O�ł����D�v�f�̃T�C�Y{}�D
			Message message(LogType::OutOfRange);
			const std::size_t arraySize = arrayRead->size<T>(key);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "__index", index, arraySize);
			consoleWrite->push(std::move(log));
			throw Internal::Exception();
		}
		else {
			FluidumScene_Log_EnumClass_Error(val);
		}

		FluidumScene_Log_InternalError();
		throw Internal::InternalError(__FILE__);
	}

	Array::pushVal<T>(L, ret);

	return 1;
}

template
FS::Lua::Ret FS::Lua::Calc::array___index<FD::Calc::Array::ValueType::Number>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array___index<FD::Calc::Array::ValueType::String>(State L);
template
FS::Lua::Ret FS::Lua::Calc::array___index<FD::Calc::Array::ValueType::Bit>(State L);
