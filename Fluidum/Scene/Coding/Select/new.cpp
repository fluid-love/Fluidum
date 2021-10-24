#include "new.h"
#include <imgui_internal.h>
#include "../TextEditor/texteditor.h"
#include "../../Utils/Popup/message.h"
#include <nfd.h>

using namespace FU::ImGui::Operators;

FS::Coding::New::New(
	FD::Coding::TabWrite* const tabWrite,
	const FD::ProjectRead* const projectRead,
	FD::ProjectWrite* const projectWrite,
	const FD::GuiRead* const guiRead
) : tabWrite(tabWrite), projectRead(projectRead), projectWrite(projectWrite)
{
	GLog.add<FD::Log::Type::None>("Construct Coding::NewScene.");

	style.windowPos = guiRead->centerPos() - (guiRead->windowSize() / 3.0f);
	style.windowSize = guiRead->windowSize() - (style.windowPos * 2.0f);

	//capacity
	folderPath.reserve(200);
	fileName.reserve(200);

	//default
	folderPath = projectRead->getProjectFolderPath() + "Src/";

}

FS::Coding::New::~New() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Coding::NewScene.");
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

void FS::Coding::New::call() {

	ImGui::SetNextWindowFocus();
	ImGui::SetNextWindowPos(style.windowPos);
	ImGui::SetNextWindowSize(style.windowSize);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove;

	//animation
	ImAnime::PushStyleVar(anime.counter, 0.5f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);

	ImGui::Begin("CodingNew", nullptr, flag);

	this->title();

	ImGui::Spacing(); ImGui::Spacing();

	//ImGui::BeginChild("Inner", { 0.0f,style.windowSize.y * 0.5f });
	//ImGui::PushItemWidth(style.windowSize.x * 0.4f);
	//this->folderPath();
	//this->projectName();
	//ImGui::PopItemWidth();
	//style.innerPosHeight = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
	//ImGui::EndChild();

	//this->bottom();

	ImGui::End();

	ImAnime::PopStyleVar();
	ImGui::PopStyleVar(3);
}

void FS::Coding::New::title() {
	ImGui::BeginChild("Title1", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.7f);
	
	ImGui::Text(projectType.c_str());
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginChild("Title2", ImVec2(style.windowSize.x / 2.0f, style.windowSize.y * 0.07f));
	ImGui::SetWindowFontScale(1.3f);

	ImGui::Text("text");

	ImGui::EndChild();
}

