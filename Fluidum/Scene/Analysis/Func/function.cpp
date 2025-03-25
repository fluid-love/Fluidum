#include "function.h"

FS::Analysis::Function::~Function() noexcept {
	FluidumScene_Log_Destructor(::FS::Analysis::Function);
}

void FS::Analysis::Function::call() {
	ImGui::Begin("Func");

	this->table();

	ImGui::End();

	ImGui::ShowDemoWindow();
}

void FS::Analysis::Function::table() {

}
