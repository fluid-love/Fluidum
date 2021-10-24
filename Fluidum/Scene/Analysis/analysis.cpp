#include "analysis.h"
#include <imgui_internal.h>

FS::AnalysisOverview::~AnalysisOverview() noexcept {
	try {
		topBarWrite->erase<AnalysisOverview>();
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

void FS::AnalysisOverview::call() {
	ImGui::Begin("AnalysisOverview",&windowShouldClose);
	this->setWindowInfo();

	this->function();
	
	ImGui::End();

	this->closeWindow();
}

void FS::AnalysisOverview::closeWindow() {
	if (windowShouldClose)
		return;

	GLog.add<FD::Log::Type::None>("Request delete AnalysisOverviewScene.");
	Scene::deleteScene<AnalysisOverview>();
}

void FS::AnalysisOverview::setWindowInfo() {
	constexpr auto code = FU::Class::ClassCode::GetClassCode<AnalysisOverview>();
	imguiWindowWrite->set(code,ImGui::GetCurrentWindow());
}

void FS::AnalysisOverview::function() {
	//ImGui::LoadIniSettingsFromDisk();
}

void FS::AnalysisOverview::topBar() {
	ImGui::Button("tes");
}






























