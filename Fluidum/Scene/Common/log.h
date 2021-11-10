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
	GLog.add<FD::Log::Type::Error>("Internal Error.")
