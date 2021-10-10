#include "lua.h"

FS::Lua::Calc::Calc(
	const FD::ProjectRead* const projectread,
	FD::ConsoleWrite* const consoleWrite
)
	: projectread(projectread), consoleWrite(consoleWrite), state(luaL_newstate())
{
	assert(this->state);
	GLog.add<FD::Log::Type::None>("Construct LuaCalcScene.");
}

FS::Lua::Calc::~Calc() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct LuaCalcScene.");

		this->terminate();
	}
	catch (const std::exception& e) {
		try {
			std::cerr << e.what() << std::endl;
			abort();
		}
		catch (...) {
			abort();
		}
	}
	catch (...) {
		abort();
	}
}

void FS::Lua::Calc::call() {

	*static_cast<Calc**>(lua_getextraspace(state)) = this;//memory

	//cppの関数を登録
	this->registerCppFunctions();
	//lua標準ライブラリの登録
	this->registerLuaLibraries();

	//読み取る
	auto error = luaL_loadfile(state, "C:/Fluidum/Test/test.lua");

	if (error != LUA_OK) {//文法errorなど
		assert(lua_isstring(state, -1));
		assert(error == LUA_ERRSYNTAX);

		GLog.add<FD::Log::Type::None>("Syntax error: {}.", luaL_tolstring(state, -1, nullptr));
	}
	else {//エラーがなければ

		GLog.add<FD::Log::Type::None>("lua_pcall().");

		//実行
		try {
			error = lua_pcall(state, 0, 0, 0);
		}
		catch (std::exception& e) {//実行時エラー
			//errorだが内部エラーではないのでType::Logで記録
			GLog.add<FD::Log::Type::None>("LuaError: {}.", e.what());
		}

	}

	GLog.add<FD::Log::Type::None>("Request delete LuaCalcScene.");
	Scene::deleteAsyncScene<Calc>();
}

void FS::Lua::Calc::registerCppFunctions() {
	using namespace Internal;

	//	const luaL_Reg genomeRegs[] = {
	//		{"Create" , &dispatch<&Lua::create_Genome>},
	//		{"SinglePointCrossOver" , &dispatch<&Lua::singlePointCrossOver>},
	//		{"Flip" , &dispatch<&Lua::flip_Genome>},
	//		{"Set" , &dispatch<&Lua::set_Genome>},
	//		{"Unset" , &dispatch<&Lua::unset_Genome>},
	//		{"ToUInt" , &dispatch<&Lua::toUInt_Genome>},
	//		{"Erase" , &dispatch<&Lua::erase_Genome>},
	//		{nullptr,nullptr}
	//	};
	//	luaL_newlib(state, genomeRegs);
	//	lua_setglobal(state, "FGenome");//Genome.
	//
	//
	//
	//	const luaL_Reg mathRegs[] = {
	//		{"RandomInt" , &dispatch<&Lua::randomInt>},
	//		{"Hypot" , &dispatch<&Lua::hypot>},
	//		{nullptr,nullptr}
	//	};
	//	luaL_newlib(state, mathRegs);
	//	lua_setglobal(state, "Math");//Math.
	//
	//
	//	const luaL_Reg plotRegs[] = {
	//		{"Create" , &dispatch<&Lua::create_Plot>},
	//		{"Push" , &dispatch<&Lua::push_Plot>},
	//		//{"Type" , &dispatch<&Lua::type>},
	//		{"Marker" , &dispatch<&Lua::marker_Plot>},
	//
	//		{nullptr,nullptr}
	//	};
	//	luaL_newlib(state, plotRegs);
	//	lua_setglobal(state, "FPlot");//FPlot.
	//
	//
	//	const luaL_Reg drawRegs[] = {
	//		{"Create" , &dispatch<&Lua::create_Draw>},
	//		{"UpdateVertex" , &dispatch<&Lua::updateVertex>},
	//		{"UpdateIndex" , &dispatch<&Lua::updateIndex>},
	//		{"Clear" , &dispatch<&Lua::clear_Draw>},
	//
	//		{nullptr,nullptr}
	//	};
	//	luaL_newlib(state, drawRegs);
	//	lua_setglobal(state, "FDraw");//FDraw.
	//

	//_Internal_System
	{
		const luaL_Reg regs[] = {
				{"SleepSeconds" , &dispatch<&Calc::sleepSeconds>},
				{"SleepMilliSeconds" , &dispatch<&Calc::sleepMilliSeconds>},
				{"Terminate" , &dispatch<&Calc::terminate>},

				{nullptr,nullptr}
		};
		luaL_newlib(state, regs);
		lua_setglobal(state, "FSystem");
	}

	//
	//
}

void FS::Lua::Calc::registerLuaLibraries() {
	luaL_openlibs(state);//standard library

	//Fluidum library
	//std::string folderPath = Resource::LuaFolderPath;
	//auto result = luaL_dofile(state, (folderPath + "system.lua").c_str());

	//assert(result == LUA_OK);


}

void FS::Lua::Calc::terminate() {

	//luaを閉じる
	int32_t num = lua_gettop(state);
	lua_pop(state, num);
	lua_close(state);

	GLog.add<FD::Log::Type::None>("Close Lua.");

}
//
//using namespace FS::LuaType;
//
//
//#pragma region Plot
//
//Ret FS::Lua::create_Plot(State L) {
//	LuaCheck::Check<LuaType::Plot::CREATE> check(L, log);
//	check.arg();
//
//	String title = lua_tostring(L, 1);
//	String axisX = lua_tostring(L, 2);
//	String axisY = lua_tostring(L, 3);
//
//	auto index = plot->addFigure(title, axisX, axisY);
//
//	LuaAssist::pop(L);
//	lua_pushinteger(L, index);
//	return 1;
//}

//Ret FS::Lua::push_Plot(State L) {
//	LuaCheck::Check<LuaType::Plot::PUSH> check(L, log);
//	check.arg();
//
//	Val figureIndex = lua_tointeger(L, 1) - 1;
//	Val dataIndex = lua_tointeger(L, 2) - 1;
//	Num x = lua_tonumber(L, 3);
//	Num y = lua_tonumber(L, 4);
//
//	auto result = plot->addPlot(figureIndex, dataIndex, x, y);
//
//	if (result != FD::Plot::Result::SUCCESS)
//		throw std::runtime_error("");
//
//	LuaAssist::pop(L);
//	return 0;
//}
//
//
//#pragma endregion
//
//
//
//
//
