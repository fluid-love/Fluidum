#pragma once

#define FluidumUtils_Debug_BeginDisableAllWarning \
    __pragma(warning(push, 0))

#define FluidumUtils_Debug_EndDisableAllWarning \
    __pragma(warning(pop))

#define FluidumUtils_Debug_BeginDisableWarning(number) \
    __pragma(warning( push )) \
    __pragma(warning( disable : number ))

#define FluidumUtils_Debug_EndDisableWarning \
    __pragma(warning( pop ))
