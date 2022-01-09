#pragma once

#include <LuAssist/utils.h>

#include "exception.h"
#include "log.h"
#include "type.h"

namespace FS::Calc::Lua::Internal {

	class Check {
	public:
		explicit Check(FD::ConsoleWrite* const consoleWrite) : consoleWrite(consoleWrite) {}

	public:
		template<FD::Calc::Lua::FunctionType Type>
		void numOfArgs(State L, const Size correctSize, const Size min, const Size max) {
			assert(min != 0);
			assert(max != 0);

			const Size size = LuAssist::Utils::numOfArgs(L);

			if (size == correctSize)
				return;

			//関数{}の引数の数が少なすぎます．渡された引数の数:{}．正しい引数の数:{}．
			if (size < min) {
				Message message(LogType::NumOfArgs_Min);
				GMessenger.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), size, correctSize);
				//consoleWrite->add
				Exception::throwException();
			}


			//関数{}の引数の数が多すぎます．渡された引数の数:{}．正しい引数の数:{}．
			if (size < min) {
				Message message(LogType::NumOfArgs_Min);
				GMessenger.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), size, correctSize);
				Exception::throwException();
			}
		}

		template<FD::Calc::Lua::FunctionType Type>
		void numOfArgs_min(State L, const Size correctSize) {
			const Size size = LuAssist::Utils::numOfArgs(L);

			if (size >= correctSize)
				return;

			Message message(LogType::NumOfArgs_Min);
			GMessenger.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), size, correctSize);
			Exception::throwException();
		}

		template<FD::Calc::Lua::FunctionType Type>
		void argType(State L, const std::vector<LuAssist::Type>& correctTypes) {
			assert(!correctTypes.empty());

			const auto types = LuAssist::Utils::types(L);

			//numOfArgs
			if (correctTypes.size() != types.size())
				Exception::throwInternalError();


			for (Size i = 0, size = types.size(); i < size; i++) {
				bool same = (correctTypes[i] == types[i]);
				if (!same) {
					//関数{}の{}番目の引数の型に誤りがあります．渡された引数の型: {}．正しい引数の型: {}．
					Message message(LogType::Type);
					GMessenger.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), i + 1, LuAssist::Utils::typeName(types[i]), LuAssist::Utils::typeName(correctTypes[i]));
					Exception::throwException();
				}
			}

		}

		template<FD::Calc::Lua::FunctionType Type>
		void argType_intNum(State L, const std::vector<LuAssist::Type>& correctTypes) {
			assert(!correctTypes.empty());

			const auto types = LuAssist::Utils::types(L);

			//numOfArgs
			if (correctTypes.size() != types.size())
				Exception::throwInternalError();

			for (Size i = 0, size = types.size(); i < size; i++) {
				bool same = (correctTypes[i] == types[i]);
				if (!same) {
					if (correctTypes[i] == LuAssist::Type::Number && types[i] == LuAssist::Type::Integer)
						;
					else {
						//関数{}の{}番目の引数の型に誤りがあります．渡された引数の型: {}．正しい引数の型: {}．
						Message message(LogType::Type);
						GMessenger.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), i + 1, LuAssist::Utils::typeName(types[i]), LuAssist::Utils::typeName(correctTypes[i]));
						Exception::throwException();
					}
				}
			}
		}

	private:
		FD::ConsoleWrite* const consoleWrite;

	};

}