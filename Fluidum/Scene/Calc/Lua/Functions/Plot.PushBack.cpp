#include "../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_pushBack(State L) {
	check.argType_intNum<Type::Plot_PushBack>(L, { CoreType::Integer,CoreType::Integer,CoreType::Number,CoreType::Number });

	Val figureKey = lua_tointeger(L, 1);
	Val plotIndex = lua_tointeger(L, 2) - 1;
	Num x = lua_tonumber(L, 3);
	Num y = lua_tonumber(L, 4);

	if (plotIndex <= 0) {
		//FPlot PlotIndex��1�ȏ�łȂ���΂Ȃ�܂���D�w�肵���l{}�D
		Message message(LogType::Plot_PlotIndex_Min);
		GLog.add<FD::Log::Type::None>(message, plotIndex);
		throw Internal::Exception();
	}

	if (plotIndex > FD::Plot::Limit::PlotMax) {
		//FPlot 1�̘g�ɕ`�ʂł���O���t�̍ő�l�𒴂���PlotIndex���ݒ肳��Ă��܂��D�ݒ肳�ꂽ�l{}�D�ő�l{}�D
		Message message(LogType::Plot_PlotIndex_Max);
		GLog.add<FD::Log::Type::None>(message, plotIndex, FD::Plot::Limit::PlotMax);

		std::rethrow_exception(std::current_exception());
	}

	implotWrite->pushBack(static_cast<FD::Plot::FigureIndex>(figureKey), static_cast<FD::Plot::PlotIndex>(plotIndex), x, y);

	return 0;
}
