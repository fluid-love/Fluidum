#pragma once

#include "boost/predef.h"

//Microsoft Visual C/C++
#ifdef BOOST_COMP_MSVC

#define FluidumScene_Log_EnumClass_Error(value)\
	GLog.add<FD::Log::Type::Error>("Failed. {}. {}." ,__FUNCSIG__, magic_enum::enum_name(value))

#else 

#define FluidumScene_Log_EnumClassException(value)\
	GLog.add<FD::Log::Type::Error>("Failed. {}. {}." ,__func__, magic_enum::enum_name(value))

#endif 


#define FluidumScene_Console_InternalError \
	this->consoleWrite->add(std::string("Internal Error."))

#define FluidumScene_Log_RequestAddScene(name) \
	GLog.add<FD::Log::Type::None>("[Request] Add " name " Scene.")

#define FluidumScene_Log_RequestTryAddScene(name) \
	GLog.add<FD::Log::Type::None>("[Request] Try add " name " Scene.")

#define FluidumScene_Log_RequestDeleteScene(name) \
	GLog.add<FD::Log::Type::None>("[Request] Delete " name " Scene.")

#define FluidumScene_Log_RequestTryDeleteScene(name) \
	GLog.add<FD::Log::Type::None>("[Request] Try delete " name " Scene.")

#define FluidumScene_Log_CallSceneConstructor(name) \
	GLog.add<FD::Log::Type::None>("[Request] Call constructor " name ".")

#define FluidumScene_Log_Abort() \
	GLog.add<FD::Log::Type::Error>("abort() has been called. File {}.", __FILE__)

#define FluidumScene_Log_InternalError() \
	GLog.add<FD::Log::Type::Error>("Internal Error. File {}.", __FILE__)

#define FluidumScene_Log_Constructor(name) \
	GLog.add<FD::Log::Type::None>("Construct " name " Scene.")

#define FluidumScene_Log_Destructor(name) \
	GLog.add<FD::Log::Type::None>("Destruct " name " Scene.")

#define FluidumScene_Log_Destructor_(name)							\
try {																\
	GLog.add<FD::Log::Type::None>("Destruct " name " Scene.");	    \
}																	\
catch (const std::exception& e) {									\
	try {															\
		std::cerr << e.what() << std::endl;							\
		abort();													\
	}																\
	catch (...) {													\
		abort();													\
	}																\
}																	\
catch (...) {														\
	abort();														\
}														