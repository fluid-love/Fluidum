#include "overview.h"
#include <imgui_internal.h>

FS::Analysis::Overview::~Overview() noexcept {
	try {
		topBarWrite->erase<Overview>();
		GLog.add<FD::Log::Type::None>("Destruct AnalysisScene.");
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

void FS::Analysis::Overview::call() {
	ImGui::Begin("AnalysisOverview",&windowShouldClose);
	this->setWindowInfo();

	this->function();
	
	ImGui::End();

	this->closeWindow();
}

void FS::Analysis::Overview::closeWindow() {
	if (windowShouldClose)
		return;

	GLog.add<FD::Log::Type::None>("Request delete AnalysisOverviewScene.");
	Scene::deleteScene<Overview>();
}

void FS::Analysis::Overview::setWindowInfo() {
	constexpr auto code = FU::Class::ClassCode::GetClassCode<Overview>();
	imguiWindowWrite->set(code,ImGui::GetCurrentWindow());
}

void FS::Analysis::Overview::function() {
	//ImGui::LoadIniSettingsFromDisk();
}

void FS::Analysis::Overview::topBar() {
	ImGui::Button("tes");
}






























