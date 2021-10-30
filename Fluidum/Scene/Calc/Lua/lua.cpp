#include "lua.h"

FS::Lua::Calc::Calc(
	const FD::ProjectRead* const projectread,
	FD::ConsoleWrite* const consoleWrite,
	FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite,
	FD::ImPlotWrite* const implotWrite,
	const FD::ImPlotRead* const implotRead
)
	: projectread(projectread),
	consoleWrite(consoleWrite),
	functionWrite(functionWrite),
	implotWrite(implotWrite),
	implotRead(implotRead),
	state(luaL_newstate())
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
		catch (const std::exception& e) {//実行時エラー
			GLog.add<FD::Log::Type::Error>("LuaInternalError: {}.", e.what());
			std::rethrow_exception(std::current_exception());
		}
		catch (const Internal::Exception) {
			//errorだが内部エラーではないのでType::Logで記録
			GLog.add<FD::Log::Type::None>("Lua RuntimeError.");
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

	//FSystem direct
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

	//_Internal_Plot_
	{
		const luaL_Reg regs[] = {
				{"Create" , &dispatch<&Calc::plot_create>},
				{"SetMarker" , &dispatch<&Calc::plot_setMarker>},
				{"PushBack" , &dispatch<&Calc::plot_pushBack>},

				{nullptr,nullptr}
		};
		luaL_newlib(state, regs);
		lua_setglobal(state, "_Internal_Plot_");
	}


}

void FS::Lua::Calc::registerLuaLibraries() {
	luaL_openlibs(state);//standard library

	//Fluidum Standard library
	std::string folderPath = Resource::LuaFolderPath;

	auto result = luaL_dofile(state, (folderPath + "plot.lua").c_str());
	assert(result == LUA_OK);


}

void FS::Lua::Calc::terminate() {

	lua_close(state);

	GLog.add<FD::Log::Type::None>("Close Lua.");

}