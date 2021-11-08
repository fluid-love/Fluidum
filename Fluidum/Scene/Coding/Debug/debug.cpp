#include "debug.h"

FS::Coding::Debug::Debug()
{
	GLog.add<FD::Log::Type::None>("Construct Coding::DebugScene.");

}

FS::Coding::Debug::~Debug() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Coding::DebugScene.");
	}
	catch (const std::exception& e) {
		try {
			std::cerr << e.what() << std::endl;
			abort();
		}
		catch (...) {
			abort();
		}
	}
	catch (...) {
		abort();
	}
}

void FS::Coding::Debug::call() {

}
