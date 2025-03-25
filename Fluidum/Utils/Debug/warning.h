#pragma once

#include <boost/predef.h>

#ifdef BOOST_COMP_MSVC

#define FluidumUtils_Debug_BeginDisableAllWarning \
    __pragma(warning(push, 0))

#define FluidumUtils_Debug_EndDisableAllWarning \
    __pragma(warning(pop))

#define FluidumUtils_Debug_BeginDisableWarning(number) \
    __pragma(warning( push )) \
    __pragma(warning( disable : number ))

#define FluidumUtils_Debug_EndDisableWarning \
    __pragma(warning( pop ))

#else

#define FluidumUtils_Debug_BeginDisableAllWarning

#define FluidumUtils_Debug_EndDisableAllWarning

#define FluidumUtils_Debug_BeginDisableWarning(number)

#define FluidumUtils_Debug_EndDisableWarning 

#endif