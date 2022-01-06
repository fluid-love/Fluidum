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

			//ŠÖ”{}‚Ìˆø”‚Ì”‚ª­‚È‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
			if (size < min) {
				Message message(LogType::NumOfArgs_Min);
				GLog.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), size, correctSize);
				//consoleWrite->add
				throw Exception();
			}


			//ŠÖ”{}‚Ìˆø”‚Ì”‚ª‘½‚·‚¬‚Ü‚·D“n‚³‚ê‚½ˆø”‚Ì”:{}D³‚µ‚¢ˆø”‚Ì”:{}D
			if (size < min) {
				Message message(LogType::NumOfArgs_Min);
				GLog.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), size, correctSize);
				throw Exception();
			}
		}

		template<FD::Calc::Lua::FunctionType Type>
		void numOfArgs_min(State L, const Size correctSize) {
			const Size size = LuAssist::Utils::numOfArgs(L);

			if (size >= correctSize)
				return;

			Message message(LogType::NumOfArgs_Min);
			GLog.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), size, correctSize);
			throw Exception();
		}

		template<FD::Calc::Lua::FunctionType Type>
		void argType(State L, const std::vector<LuAssist::Type>& correctTypes) {
			assert(!correctTypes.empty());

			const auto types = LuAssist::Utils::types(L);

			//numOfArgs
			if (correctTypes.size() != types.size())
				throw Internal::InternalError(__FILE__);


			for (Size i = 0, size = types.size(); i < size; i++) {
				bool same = (correctTypes[i] == types[i]);
				if (!same) {
					//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½ˆø”‚ÌŒ^: {}D³‚µ‚¢ˆø”‚ÌŒ^: {}D
					Message message(LogType::Type);
					GLog.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), i + 1, LuAssist::Utils::typeName(types[i]), LuAssist::Utils::typeName(correctTypes[i]));
					throw Exception();
				}
			}

		}

		template<FD::Calc::Lua::FunctionType Type>
		void argType_intNum(State L, const std::vector<LuAssist::Type>& correctTypes) {
			assert(!correctTypes.empty());

			const auto types = LuAssist::Utils::types(L);

			//numOfArgs
			if (correctTypes.size() != types.size())
				throw Internal::InternalError(__FILE__);

			for (Size i = 0, size = types.size(); i < size; i++) {
				bool same = (correctTypes[i] == types[i]);
				if (!same) {
					if (correctTypes[i] == LuAssist::Type::Number && types[i] == LuAssist::Type::Integer)
						;
					else {
						//ŠÖ”{}‚Ì{}”Ô–Ú‚Ìˆø”‚ÌŒ^‚ÉŒë‚è‚ª‚ ‚è‚Ü‚·D“n‚³‚ê‚½ˆø”‚ÌŒ^: {}D³‚µ‚¢ˆø”‚ÌŒ^: {}D
						Message message(LogType::Type);
						GLog.add_str<FU::Log::Type::None>(message, magic_enum::enum_name<Type>(), i + 1, LuAssist::Utils::typeName(types[i]), LuAssist::Utils::typeName(correctTypes[i]));
						throw Exception();
					}
				}
			}
		}

	private:
		FD::ConsoleWrite* const consoleWrite;

	};

}