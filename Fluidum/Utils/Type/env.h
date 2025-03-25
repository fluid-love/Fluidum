#pragma once

#include <boost/predef.h>

#ifdef BOOST_OS_WINDOWS
#define FluidumUtils_Type_OS_Windows
#endif

#ifdef BOOST_OS_MACOS
#define FluidumUtils_Type_OS_MacOS
#endif

#ifdef BOOST_OS_LINUX
#define FluidumUtils_Type_OS_Linux
#endif