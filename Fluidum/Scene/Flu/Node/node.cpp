#include "node.h"

FS::Flu::Node::Node()
{
	GLog.add<FD::Log::Type::None>("Construct Flu::NodeScene.");

}

FS::Flu::Node::~Node() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Flu::NodeScene.");
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

void FS::Flu::Node::call() {

}
