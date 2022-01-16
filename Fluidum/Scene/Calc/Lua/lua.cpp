#include "lua.h"

FS::Calc::Lua::Run::Run(
	const FD::ProjectRead* const projectRead,
	FD::ConsoleWrite* const consoleWrite,
	FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite,
	FD::ImPlotWrite* const implotWrite,
	const FD::ImPlotRead* const implotRead,
	FD::Calc::ArrayWrite* const arrayWrite,
	const FD::Calc::ArrayRead* const arrayRead,
	const std::string& entryFilePath
) :
	projectRead(projectRead),
	consoleWrite(consoleWrite),
	functionWrite(functionWrite),
	implotWrite(implotWrite),
	implotRead(implotRead),
	arrayWrite(arrayWrite),
	arrayRead(arrayRead),

	entryFilePath(entryFilePath),

	state(luaL_newstate())
{
	assert(this->state);
	FluidumScene_Log_Constructor(::FS::Calc::Lua::Run);
}

FS::Calc::Lua::Run::~Run() noexcept {
	FluidumScene_Log_Destructor(::FS::Calc::Lua::Run);
}

void FS::Calc::Lua::Run::call() {

	*static_cast<Run**>(lua_getextraspace(state)) = this;//memory

	this->registerCppFunctions();

	{
		bool res = this->init();
		if (!res) {
			this->terminate();
			return;
		}
	}

	//load main file
	if (!entryFilePath.empty()) {
		this->terminate();
		return;
	}

	auto error = luaL_loadfile(state, this->entryFilePath.c_str());

	if (error != LUA_OK) {//syntax error ,...
		assert(lua_isstring(state, -1));
		assert(error == LUA_ERRSYNTAX);

		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Syntax error: {}.", luaL_tolstring(state, -1, nullptr));
	}
	else {//no_error

		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "lua_pcall().");

		//run
		try {
			error = lua_pcall(state, 0, 0, 0);
		}
		catch (const std::exception& e) {//runtime error
			FluidumScene_Log_StdExceptionWarning(e);
		}
		catch (const Internal::Exception::Exception&) {
			GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Lua RuntimeError.");
		}
		catch (const Internal::Exception::InternalError&) {
			GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Lua Fluidum InternalError.");
		}
	}

	if (error != LUA_OK) {
		GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, lua_tostring(state, -1));
	}

	this->terminate();
}

void FS::Calc::Lua::Run::registerCppFunctions() {
	using namespace Internal;

	//_Fluidum_Register_CppFunctions_
	{
		const luaL_Reg regs[] = {
			{"FSLpath" , &dispatch<&Run::_Fluidum_Register_CppFunctions_FSLPath> },

			{"System"   , &dispatch<&Run::_Fluidum_Register_CppFunctions_System>  },
			{"Array"   , &dispatch<&Run::_Fluidum_Register_CppFunctions_Array>   },
			{"Plot"    , &dispatch<&Run::_Fluidum_Register_CppFunctions_Plot>    },

			{nullptr , nullptr}
		};
		luaL_newlib(state, regs);
		lua_setglobal(state, "_Fluidum_Register_CppFunctions_");
	}


	//libraries math,...
	luaL_openlibs(state);
}

void FS::Calc::Lua::Run::terminate() {
	lua_close(state);
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Close Lua.");

	FluidumScene_Log_RequestDeleteScene(::FS::Calc::Lua::Run);
	Scene::deleteAsyncScene<Run>();
}

FS::Calc::Lua::Ret FS::Calc::Lua::Run::_Fluidum_Register_CppFunctions_FSLPath(State L) {
	assert(LuAssist::Utils::empty(L));

	lua_pushstring(L, Resource::LuaFluidumStandardLibraryFolderPath);

	return 1;
}

bool FS::Calc::Lua::Run::init() {
	const auto result = luaL_dofile(state, Resource::LuaInitFilePath);

	if (result == LUA_OK)
		return true;

	GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Failed to init Lua.");
	Internal::Message message(Internal::LogType::Initialize);
	consoleWrite->push(message.operator const std::string & ());
	return false;
}

FS::Calc::Lua::Ret FS::Calc::Lua::Run::_Fluidum_Register_CppFunctions_System(State L) {
	using namespace Internal;

	assert(LuAssist::Utils::size(L) == 0);

	const luaL_Reg regs[] = {
		{"print" , &dispatch<&Run::system_print> },
		//{"print" , &dispatch<&Calc::array_make> },
		//{"print" , &dispatch<&Calc::array_make> },


		{nullptr , nullptr                        }
	};
	luaL_newlib(state, regs);

	return 1;
}

FS::Calc::Lua::Ret FS::Calc::Lua::Run::_Fluidum_Register_CppFunctions_Array(State L) {
	using namespace Internal;
	using namespace FD::Calc::Array;

	assert(LuAssist::Utils::size(L) == 0);

	const luaL_Reg regs[] = {
		{"make"                , &dispatch<&Run::array_make>                            },

		{"empty_number"        , &dispatch<&Run::array_empty<ValueType::Number>>        },
		{"empty_string"        , &dispatch<&Run::array_empty<ValueType::String>>        },
		{"empty_bit"           , &dispatch<&Run::array_empty<ValueType::Bit>>           },

		{"size_number"         , &dispatch<&Run::array_size<ValueType::Number>>         },
		{"size_string"         , &dispatch<&Run::array_size<ValueType::String>>         },
		{"size_bit"            , &dispatch<&Run::array_size<ValueType::Bit>>            },

		{"front_number"        , &dispatch<&Run::array_front<ValueType::Number>>        },
		{"front_string"        , &dispatch<&Run::array_front<ValueType::String>>        },
		{"front_bit"           , &dispatch<&Run::array_front<ValueType::Bit>>           },

		{"back_number"         , &dispatch<&Run::array_back<ValueType::Number>>         },
		{"back_string"         , &dispatch<&Run::array_back<ValueType::String>>         },
		{"back_bit"            , &dispatch<&Run::array_back<ValueType::Bit>>            },

		{"__index_number"      , &dispatch<&Run::array___index<ValueType::Number>>      },
		{"__index_string"      , &dispatch<&Run::array___index<ValueType::String>>      },
		{"__index_bit"         , &dispatch<&Run::array___index<ValueType::Bit>>         },

		{"__newindex_number"   , &dispatch<&Run::array___newindex<ValueType::Number>>   },
		{"__newindex_string"   , &dispatch<&Run::array___newindex<ValueType::String>>   },
		{"__newindex_bit"      , &dispatch<&Run::array___newindex<ValueType::Bit>>      },

		{"set_number"          , &dispatch<&Run::array_set<ValueType::Number>>          },
		{"set_string"          , &dispatch<&Run::array_set<ValueType::String>>          },
		{"set_bit"             , &dispatch<&Run::array_set<ValueType::Bit>>             },

		{"push_front_number"   , &dispatch<&Run::array_push_front<ValueType::Number>>   },
		{"push_front_string"   , &dispatch<&Run::array_push_front<ValueType::String>>   },
		{"push_front_bit"      , &dispatch<&Run::array_push_front<ValueType::Bit>>      },

		{"push_back_number"    , &dispatch<&Run::array_push_back<ValueType::Number>>    },
		{"push_back_string"    , &dispatch<&Run::array_push_back<ValueType::String>>    },
		{"push_back_bit"       , &dispatch<&Run::array_push_back<ValueType::Bit>>       },

		{"pop_front_number"    , &dispatch<&Run::array_pop_front<ValueType::Number>>    },
		{"pop_front_string"    , &dispatch<&Run::array_pop_front<ValueType::String>>    },
		{"pop_front_bit"       , &dispatch<&Run::array_pop_front<ValueType::Bit>>       },

		{"pop_back_number"     , &dispatch<&Run::array_pop_back<ValueType::Number>>     },
		{"pop_back_string"     , &dispatch<&Run::array_pop_back<ValueType::String>>     },
		{"pop_back_bit"        , &dispatch<&Run::array_pop_back<ValueType::Bit>>        },

		{"clear_number"        , &dispatch<&Run::array_clear<ValueType::Number>>        },
		{"clear_string"        , &dispatch<&Run::array_clear<ValueType::String>>        },
		{"clear_bit"           , &dispatch<&Run::array_clear<ValueType::Bit>>           },

		{"erase_number"        , &dispatch<&Run::array_erase<ValueType::Number>>        },
		{"erase_string"        , &dispatch<&Run::array_erase<ValueType::String>>        },
		{"erase_bit"           , &dispatch<&Run::array_erase<ValueType::Bit>>           },

		//{"erase_number"        , &dispatch<&Calc::array_erase<ValueType::Number>>        },
		//{"erase_string"        , &dispatch<&Calc::array_erase<ValueType::String>>        },
		//{"erase_bit"           , &dispatch<&Calc::array_erase<ValueType::Bit>>           },

		{"__gc_number"		   , &dispatch<&Run::array___gc<ValueType::Number>>         },
		{"__gc_string"		   , &dispatch<&Run::array___gc<ValueType::String>>         },
		{"__gc_bit"			   , &dispatch<&Run::array___gc<ValueType::Bit>>            },

		{nullptr               , nullptr                                                 }
	};
	luaL_newlib(state, regs);

	return 1;
}

FS::Calc::Lua::Ret FS::Calc::Lua::Run::_Fluidum_Register_CppFunctions_Plot(State L) {


	return 0;
}

