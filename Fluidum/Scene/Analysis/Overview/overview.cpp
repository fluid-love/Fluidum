#include "overview.h"
#include <imgui_internal.h>

FS::Analysis::Overview::~Overview() noexcept {
	try {
		toolBarWrite->remove<Overview>();
		FluidumScene_Log_Destructor("Analysis::Overview");
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
	ImGui::Begin("AnalysisOverview", &windowShouldClose);
	this->setWindowInfo();

	this->tab();

	ImGui::End();

	this->closeWindow();
}

void FS::Analysis::Overview::closeWindow() {
	if (windowShouldClose)
		return;

	FluidumScene_Log_RequestDeleteScene("Analysis::Overview");
	Scene::deleteScene<Overview>();
}

void FS::Analysis::Overview::setWindowInfo() {
	constexpr auto code = FU::Class::ClassCode::GetClassCode<Overview>();
	imguiWindowWrite->set(code, ImGui::GetCurrentWindow());
}

void FS::Analysis::Overview::tab() {
	if (!ImGui::BeginTabBar("AnaOverview"))
		return;

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_TabActive, { 0.2f,0.3f,0.75f,0.3f });

	if (ImGui::BeginTabItem("plot")) {
		this->plot();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("func")) {
		this->function();
		ImGui::EndTabItem();
	}

	ImGui::PopStyleColor();

	ImGui::EndTabBar();
}

void FS::Analysis::Overview::empty() {
	ImGui::Text("表示可能なデータがありません．");
}

void FS::Analysis::Overview::plot() {
	if (implotRead->empty()) {
		this->empty();
		return;
	}

	if (!ImGui::BeginTable("table2", 2,ImGuiTableFlags_Borders))
		return;

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("FigureSize %d", implotRead->figureSize());
	ImGui::TableNextColumn();
	ImGui::Text("Some contents");

	ImGui::EndTable();

}

void FS::Analysis::Overview::function() {
	//ImGui::LoadIniSettingsFromDisk();
}

void FS::Analysis::Overview::topBar() {
	ImGui::Button("tes");
}






























