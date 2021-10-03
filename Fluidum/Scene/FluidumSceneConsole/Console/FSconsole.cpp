#include "FSconsole.h"

FS::Console::Console(
	Manager* manager,
	FD::LogWrite* log,
	FD::ConsoleLogRead* console
)
	:Scene(manager), log(log), console(console)
{
	FluidumUtils_ConsoleLog_Constructor(Console)
}

FS::Console::~Console() {
	FluidumUtils_ConsoleLog_Destructor(Console)
}

void FS::Console::update() {

}

void FS::Console::draw() {

}
