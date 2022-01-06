#pragma once

#include "boost/predef.h"

//Microsoft Visual C/C++
#ifdef BOOST_COMP_MSVC

#define FluidumScene_Log_EnumClass_Error(value)\
	GLog.add_str<FU::Log::Type::Error>("Failed. {}. {}." ,__FUNCSIG__, magic_enum::enum_name(value))

#else 

#define FluidumScene_Log_EnumClassException(value)\
	GLog.add_str<FU::Log::Type::Error>("Failed. {}. {}." ,__func__, magic_enum::enum_name(value))

#endif 


#define FluidumScene_Console_InternalError \
	this->consoleWrite->push(std::string("Internal Error."))

#define FluidumScene_Log_RequestAddScene(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_RequestAddScene");	   \
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Add " #name " Scene.")

#define FluidumScene_Log_RequestTryAddScene(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_RequestTryAddScene");  \
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Try add " #name " Scene.")

#define FluidumScene_Log_RequestDeleteScene(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_RequestDeleteScene");  \
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Delete " #name " Scene.")

#define FluidumScene_Log_RequestTryDeleteScene(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_RequestTryDeleteScene");  \
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Try delete " #name " Scene.")

#define FluidumScene_Log_CallSceneConstructor(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_CallSceneConstructor");  \
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "[Request] Call constructor " #name ".")

#define FluidumScene_Log_InternalError() \
	GLog.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.")

#define FluidumScene_Log_InternalWarning() \
	GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "Internal Error.")

#define FluidumScene_Log_Constructor(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_Constructor");  \
	GLog.add_str<FU::Log::Type::None>("Construct " #name " Scene.")

#define FluidumScene_Log_Destructor(name) \
	static_assert(FU::Concept::IsClass<name>, "FluidumScene_Log_Destructor");  \
	GLog.add_str<FU::Log::Type::None>("Destruct " #name " Scene.")

#define FluidumScene_Log_StdExceptionError(err) \
	GLog.add<FU::Log::Type::Error>(__FILE__, __LINE__, "std::exception was thrown({}).", err.what())

#define FluidumScene_Log_StdExceptionWarning(err) \
	GLog.add<FU::Log::Type::Warning>(__FILE__, __LINE__, "std::exception was thrown({}).", err.what())
