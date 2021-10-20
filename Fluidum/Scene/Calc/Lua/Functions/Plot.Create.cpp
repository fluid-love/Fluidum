#include "../lua.h"

FS::Lua::Ret FS::Lua::Calc::plot_create(State L) {
	check.argType<Type::None>(L, { CoreType::String,CoreType::String,CoreType::String });

	String title = lua_tostring(L, 1);
	String axisX = lua_tostring(L, 2);
	String axisY = lua_tostring(L, 3);

	if (implotRead->figureSize() >= FD::Plot::Limit::FigureMax) {
		//ŠÖ”FPlot.Create‚ªŒÄ‚Î‚ê‚Ü‚µ‚½‚ªì¬‚Å‚«‚éFigure‚ÌÅ‘å”‚ğ’´‚¦‚Ü‚µ‚½DÅ‘å”{}D
		Message message(LogType::Plot_Figure_Max);
		GLog.add<FD::Log::Type::None>(message, FD::Plot::Limit::FigureMax);
		throw Internal::Exception();
	}

	const FD::Plot::FigureIndex index = implotWrite->addFigure(title, axisX, axisY);

	Lua::pop(L);
	lua_pushinteger(L, index);
	return 1;
}
