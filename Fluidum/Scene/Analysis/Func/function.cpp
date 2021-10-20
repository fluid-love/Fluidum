#include "function.h"

FS::Analysis::Function::~Function() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct PlotScene.");
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

void FS::Analysis::Function::call() {
	ImGui::Begin("Func");

	this->table();

	ImGui::End();

	ImGui::ShowDemoWindow();
}

void FS::Analysis::Function::table() {

}
