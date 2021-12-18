#include "message.h"

FS::Utils::Message::Message(
	const FU::GuiText::Text& message,
	const ImVec2& pos,
	const ImVec4& bgColor
) : message(message), pos(pos), bgColor(bgColor)
{
	FluidumScene_Log_Constructor("Utils::Message");
}

FS::Utils::Message::~Message() noexcept {
	FluidumScene_Log_Destructor_("Utils::Message");
}

void FS::Utils::Message::call() {

	ImGui::SetNextWindowPos(pos);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4.0f);

	ImGui::PushStyleColor(ImGuiCol_PopupBg, bgColor);
	ImAnime::PushStyleVar(counter, 0.7f, 0.0f, 1.0f, ImAnimeType::LINEAR, ImGuiStyleVar_Alpha);
	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_Tooltip
		;

	ImGui::Begin("Message", nullptr, flag);
	ImGui::Text(message);
	ImGui::End();

	ImAnime::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		Scene::deleteScene<Message>();
	}

}
