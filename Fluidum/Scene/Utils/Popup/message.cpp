#include "message.h"

FS::Utils::Message::Message(
	const FU::GuiText::Text& message,
	const ImVec2& pos,
	const ImVec4& bgColor
) : message(message), pos(pos), bgColor(bgColor)
{
	GLog.add<FD::Log::Type::None>("Construct Utils::MessageScene.");
}

FS::Utils::Message::~Message() noexcept {
	try {
		GLog.add<FD::Log::Type::None>("Destruct Utils::MessageScene.");
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

void FS::Utils::Message::call() {

	ImGui::SetNextWindowPos(pos);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 4.0f);

	ImGui::PushStyleColor(ImGuiCol_PopupBg, bgColor);

	constexpr auto flag =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_Tooltip
		;

	ImGui::Begin("Message", nullptr, flag);
	ImGui::TextUnformatted(message);
	ImGui::End();


	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
		Scene::deleteScene<Message>();
	}

}
