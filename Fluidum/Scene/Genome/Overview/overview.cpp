#include "overview.h"

FS::Genome::Overview::Overview()
{
	GLog.add<FD::Log::Type::None>("Construct Genome::OverviewScene.");

}

FS::Genome::Overview::~Overview() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Genome::OverviewScene.");
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

void FS::Genome::Overview::call() {

}
