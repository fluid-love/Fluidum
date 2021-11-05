#include "animation.h"

FS::Animation::Animation() {

	GLog.add<FD::Log::Type::None>("Construct AnimationScene.");

}

FS::Animation::~Animation() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct AnimationScene.");
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

void FS::Animation::call() {

}
