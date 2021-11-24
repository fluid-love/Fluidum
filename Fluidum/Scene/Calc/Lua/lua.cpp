#include "lua.h"

FS::Lua::Calc::Calc(
	const FD::ProjectRead* const projectread,
	FD::ConsoleWrite* const consoleWrite,
	FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite,
	FD::ImPlotWrite* const implotWrite,
	const FD::ImPlotRead* const implotRead,
	FD::Calc::ArrayWrite* const arrayWrite,
	const FD::Calc::ArrayRead* const arrayRead
)
	: projectread(projectread),
	consoleWrite(consoleWrite),
	functionWrite(functionWrite),
	implotWrite(implotWrite),
	implotRead(implotRead),
	arrayWrite(arrayWrite),
	arrayRead(arrayRead),
	state(luaL_newstate())
{
	assert(this->state);
	FluidumScene_Log_Constructor("Lua::Calc");
}

FS::Lua::Calc::~Calc() noexcept {
	FluidumScene_Log_Destructor_("Lua::Calc");
}

void FS::Lua::Calc::call() {

	*static_cast<Calc**>(lua_getextraspace(state)) = this;//memory

	this->registerCppFunctions();

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
		catch (const Internal::InternalError& e) {
			GLog.add<FD::Log::Type::Error>("Lua Fluidum InternalError. File {}.", e.file);
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
	//{
	//	const luaL_Reg regs[] = {
	//			{"sleep_seconds"      , &dispatch<&Calc::system_sleep_seconds>      },
	//			{"sleep_milliseconds" , &dispatch<&Calc::system_sleep_milliseconds> },
	//			{"terminate"          , &dispatch<&Calc::system_terminate>          },

	//			{nullptr,nullptr}
	//	};
	//	luaL_newlib(state, regs);
	//	lua_setglobal(state, "FSystem");
	//}

	////_Internal_Plot_
	//{
	//	const luaL_Reg regs[] = {
	//			{"make"       , &dispatch<&Calc::plot_make>      },
	//			{"make_plot"  , &dispatch<&Calc::plot_make_plot> },
	//			{"plot"       , &dispatch<&Calc::plot_plot>      },
	//			{"marker"     , &dispatch<&Calc::plot_marker>    },
	//			{"push_back"  , &dispatch<&Calc::plot_push_back> },

	//			{nullptr,nullptr}
	//	};
	//	luaL_newlib(state, regs);
	//	lua_setglobal(state, "_Internal_Plot_");
	//}

	//_Fluidum_Register_CppFunctions_
	const luaL_Reg regs[] = {
		{"Array" , &dispatch<&Calc::_Fluidum_Register_CppFunctions_Array> },
		{"Plot"  , &dispatch<&Calc::_Fluidum_Register_CppFunctions_Plot>  },

		{nullptr , nullptr}
	};
	luaL_newlib(state, regs);
	lua_setglobal(state, "_Fluidum_Register_CppFunctions_");
}

void FS::Lua::Calc::terminate() {
	lua_close(state);
	GLog.add<FD::Log::Type::None>("Close Lua.");
}

FS::Lua::Ret FS::Lua::Calc::_Fluidum_Register_CppFunctions_Array(State L) {
	using namespace Internal;

	assert(LuAssist::Utils::size(L) == 0);

	const luaL_Reg regs[] = {
		{"make"  , &dispatch<&Calc::plot_make>      },
		{nullptr ,nullptr}
	};
	luaL_newlib(state, regs);

	return 1;
}

FS::Lua::Ret FS::Lua::Calc::_Fluidum_Register_CppFunctions_Plot(State L) {


	return 0;
}

