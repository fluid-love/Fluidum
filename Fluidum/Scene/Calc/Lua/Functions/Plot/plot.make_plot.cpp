#include "../../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_make_plot(State L) {
	check.argType<Type::Plot_makePlot>(L, { LuAssist::Type::Integer });

	Val figureIndex = lua_tointeger(L, 1);

	assert(figureIndex >= 0 && figureIndex < std::numeric_limits<FD::Plot::FigureIndex>::max());

	FD::Plot::PlotIndex index;

	try {
		index = implotWrite->addPlot(static_cast<FD::Plot::FigureIndex>(figureIndex));
	}
	catch (const FD::ImPlotWrite::Exception value) {
		using enum FD::ImPlotWrite::Exception;

		if (value == FigureSize) {
			FluidumScene_Log_EnumClass_Error(value);
		}
		else if (value == PlotSize) {
			//{}�֐�MakePlot���Ă΂�܂������쐬�ł���Plot�̍ő吔�𒴂��܂����D�ő吔{}�D
			Message message(LogType::Plot_Figure_Max);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), FD::Plot::Limits::Plot::PlotMax);
			consoleWrite->add(std::move(log));
			throw Internal::Exception();
		}

		FluidumScene_Console_InternalError;
		throw Lua::Internal::InternalError(__FILE__);
	}


	//plot
	if (lua_istable(L, 2) && lua_istable(L, 3)) {
		//x values
		std::vector<Num> x{};
		try {
			x = LuAssist::Utils::Table::convertTableToArrayOfNumbers(L, 2);
		}
		catch (const LuAssist::Utils::Table::Exception::NotNumber& info) {
			//{}�֐�{}��{}�Ԗڂ̈����ɓn���ꂽ�e�[�u����{}�Ԗڂ̗v�f�̌^�Ɍ�肪����܂��D�n���ꂽ�^:{}�D�������^:{}�D
			Message message(LogType::TableType);
			std::string log = GLog.add<FD::Log::Type::None>(message, LuAssist::Utils::getSrcCurrentLine(L, 2), "MakePlot", 2, info.index, info.input, LuAssist::Utils::typeName(LuAssist::Type::Number));
			consoleWrite->add(std::move(log));

			implotWrite->popBackPlot(static_cast<FD::Plot::FigureIndex>(figureIndex));

			throw Internal::Exception();
		}

		//y values
		std::vector<Num> y{};
		try {
			y = LuAssist::Utils::Table::convertTableToArrayOfNumbers(L, 3);
		}
		catch (const LuAssist::Utils::Table::Exception::NotNumber& info) {
			//{}�֐�{}��{}�Ԗڂ̈����ɓn���ꂽ�e�[�u����{}�Ԗڂ̗v�f�̌^�Ɍ�肪����܂��D�n���ꂽ�^:{}�D�������^:{}�D
			Message message(LogType::TableType);
			std::string log = GLog.add<FD::Log::Type::None>(
				message,
				LuAssist::Utils::getSrcCurrentLine(L, 2),
				"MakePlot",
				3,
				info.index, 
				info.input,
				LuAssist::Utils::typeName(LuAssist::Type::Number)
				);
			consoleWrite->add(std::move(log));

			implotWrite->popBackPlot(static_cast<FD::Plot::FigureIndex>(figureIndex));

			throw Internal::Exception();
		}

		//set
		try {
			implotWrite->setPlot(static_cast<FD::Plot::FigureIndex>(figureIndex), index, std::move(x), std::move(y));
		}
		catch (const FD::ImPlotWrite::Exception val) {
			using enum FD::ImPlotWrite::Exception;
			if (val == FigureSize || val == PlotSize) {
				FluidumScene_Log_EnumClass_Error(val);
			}
			else if (val == SameSize) {
				//{}�֐�{}�ɓn�����x,y�̗v�f���͓������Ȃ���΂Ȃ�܂���D
				Message message(LogType::Plot_SameSize);
				std::string log = GLog.add<FD::Log::Type::None>(
					message,
					LuAssist::Utils::getSrcCurrentLine(L, 2),
					"MakePlot",
					LuAssist::Utils::getSrcCurrentLine(L, 2)
					);
				consoleWrite->add(std::move(log));

				implotWrite->popBackPlot(static_cast<FD::Plot::FigureIndex>(figureIndex));

				throw Internal::Exception();
			}

			implotWrite->popBackPlot(static_cast<FD::Plot::FigureIndex>(figureIndex));
			FluidumScene_Console_InternalError;
			throw Lua::Internal::InternalError(__FILE__);
		}
	}

	LuAssist::Utils::popAll(L);
	lua_pushinteger(L, index);
	return 1;
}
