#pragma once

#include <iostream>

#ifndef NDEBUG

//Constructor
#define FluidumUtils_Debug_ConsoleLog_Constructor(ClassType)\
std::cout << "Constructor: " << typeid(ClassType).name() << std::endl;\
static_assert(std::is_class_v<ClassType>,"Arg of FluidumUtils_Debug_ConsoleLog_Constructor must be Class");

//Destructor
#define FluidumUtils_Debug_ConsoleLog_Destructor(ClassType)\
std::cout << "Destructor: " << typeid(ClassType).name() << std::endl;\
static_assert(std::is_class_v<ClassType>,"Arg of FluidumUtils_ConsoleLog_Destructor must be Class．");

//any
#define FluidumUtils_Debug_ConsoleLog(message)\
std::cout << message << std::endl;

#else

#define FluidumUtils_Debug_ConsoleLog_Constructor(dummy)
#define FluidumUtils_Debug_ConsoleLog_Destructor(dummy)

#endif

