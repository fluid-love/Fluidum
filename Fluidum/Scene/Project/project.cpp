#include "project.h"

FS::Project::Project(
	FD::ProjectWrite* const projectWrite,
	const FD::ProjectRead* const projectRead,
	const FD::SceneRead* const sceneRead
) : projectWrite(projectWrite), projectRead(projectRead), sceneRead(sceneRead)
{
	GLog.add<FD::Log::Type::None>("Construct ProjectScene.");

	style.topBarHeight = ImGui::CalcTextSize(ICON_MD_FOLDER_OPEN).x + 5.0f;
}

FS::Project::~Project() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct ProjectScene.");
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

void FS::Project::Project::call() {
	ImGui::Begin("Project");
	this->topBar();
	ImGui::End();
}

void FS::Project::Project::topBar() {

	ImGui::BeginChild("ProjectTopBar", { ImGui::GetWindowSize().x,style.topBarHeight });

	//sync
	if (ImGui::Button(ICON_MD_SYNC))
		this->tab_sync();

	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	if (ImGui::Button(ICON_MD_CODE))
		this->tab_displayCode();

	//ImGui::SameLine();
	//ImGui::Spacing();
	//ImGui::SameLine();

	//if (ImGui::Button(ICON_MD_SAVE))
	//	this->save();
	//ImGui::SameLine();

	//if (ImGui::Button(ICON_MD_SYNC))
	//	this->sync();
	//ImGui::SameLine();

	//if (ImGui::Button(ICON_MD_CODE))
	//	this->code();

	ImGui::EndChild();

	ImGui::Separator();
	ImGui::Spacing();

}

void FS::Project::Project::tab_sync() {

}

void FS::Project::Project::tab_displayCode() {

}

