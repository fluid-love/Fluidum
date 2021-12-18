#include "../../lua.h"

namespace FS::Lua {
	constexpr const char* ArrayValueNames[] = {
		"number","Number","num","Num",
		"string","String","str","Str",
		"bit","Bit","binary","Binary",
	};

	std::string concatArrayValueNames() {
		std::string ret{};
		for (auto x : ArrayValueNames) {
			(ret += x) += ',';
		}
		return ret;
	}

	std::optional<FD::Calc::Array::ValueType> stringToArrayValueType(const std::string& arg) {

		using enum FD::Calc::Array::ValueType;
		constexpr FD::Calc::Array::ValueType types[] = {
			Number,
			String,
			Bit
		};

		for (uint16_t i = 0, j = 0; i < std::extent_v<decltype(ArrayValueNames), 0>; i++) {
			if (arg == ArrayValueNames[i])
				return types[i];

			if ((i + 1) % 4 == 0)
				j++;
		}

		return std::nullopt;
	}
}

FS::Lua::Ret FS::Lua::Calc::array_make(State L) {
	const bool argTypes = LuAssist::Utils::checkArgTypes<LuAssist::Type::String>(L);
	if (!argTypes) {
		//{}�֐�{}��{}�Ԗڂ̈����̌^�Ɍ�肪����܂��D�n���ꂽ�����̌^: {}�D�����������̌^: {}�D
		Message message(LogType::Type);
		const auto types = LuAssist::Utils::types(L);
		assert(types.size() == 1);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "make", 1, LuAssist::Utils::typeName(types.at(0)), LuAssist::Utils::typeName(LuAssist::Type::String));
		consoleWrite->push(std::move(log));
	}

	const String str = lua_tostring(L, 1);
	const auto type = stringToArrayValueType(str);

	if (!type.has_value()) {//not match
		//{}�֐�{}�̈����ɖ����ȕ����񂪓n����܂����D�n���ꂽ������{}�D������������{}�D
		Message message(LogType::NotMatch);
		std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), str, concatArrayValueNames());
		consoleWrite->push(std::move(log));
		throw Internal::Exception();
	}

	FD::Calc::Array::Key key;

	try {
		key = arrayWrite->make(type.value());
	}
	catch (const FD::Calc::ArrayWrite::Exception val) {
		using enum FD::Calc::ArrayWrite::Exception;
		if (val == MaxSize) {
			//{}�֐�{}���Ă΂�܂������쐬�ł���{}�̍ő吔�𒴂��Ă��܂��D�ő�l{}�D
			Message message(LogType::NotMatch);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "make", FD::Calc::Array::Limits::MaxSize);
			consoleWrite->push(std::move(log));
			throw Internal::Exception();
		}
		else {
			FluidumScene_Log_EnumClass_Error(val);
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}

	lua_pushinteger(L, key);

	{
		using enum FD::Calc::Array::ValueType;
		if (type == Number)
			lua_pushstring(L, "n");
		else if (type == String)
			lua_pushstring(L, "s");
		else
			lua_pushstring(L, "b");
	}
	return 2;
}
