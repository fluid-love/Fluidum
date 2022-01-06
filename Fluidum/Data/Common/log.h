#pragma once

#include "resource.h"

//Log
namespace FD::Internal {

	inline FU::Log::Messenger GMessenger{};

}

#define FluidumData_Log_Internal_InternalError() \
::FD::Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Internal Error.");

#define FluidumData_Log_Internal_StdExceptionError(err) \
::FD::Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "std::exception was thrown({}).", err.what());
