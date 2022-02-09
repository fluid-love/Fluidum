#include "debug.h"

FS::Coding::Debug::Debug()
{
	FluidumScene_Log_Constructor(::FS::Coding::Debug);
}

FS::Coding::Debug::~Debug() {
	FluidumScene_Log_Destructor(::FS::Coding::Debug);
}

void FS::Coding::Debug::call() {

}
