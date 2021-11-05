#include "console.h"

FS::Console::Console(
	FD::ConsoleWrite* const consoleWrite,
	const FD::ConsoleRead* const consoleRead
) :
	consoleWrite(consoleWrite),
	consoleRead(consoleRead)
{
	GLog.add<FD::Log::Type::None>("Construct ConsoleScene.");

	if (inputText.max_size() >= 2000)
		inputText.reserve(2000);
	else
		inputText.reserve(inputText.max_size());

}

FS::Console::~Console() {
	try {
		GLog.add<FD::Log::Type::None>("Destruct ConsoleScene.");
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

void FS::Console::call() {
	ImGui::Begin("Console");

	this->console();
	this->input();

	this->changeFontSize();
	ImGui::End();

}

void FS::Console::console() {
	ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.01f,0.01f,0.01f,1.0f });

	const auto size = ImGui::GetWindowSize();
	ImGui::BeginChild("Console", { size.x * 0.98f,size.y * 0.8f }, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::SetWindowFontScale(style.fontSizeScale);

	ImGuiListClipper clipper;
	clipper.Begin(100);
	while (clipper.Step()) {

		for (int32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {

			const auto info = consoleRead->get(i);
			if(info.first)
				ImGui::Text(info.second.message.c_str());
		}
	}

	ImGui::EndChild();

	ImGui::PopStyleColor();
}

void FS::Console::input() {
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (ImGui::GetStyle().WindowPadding.x * 2.0f));
	ImGui::InputText("##InputText", inputText.data(), inputText.size());
}

void FS::Console::changeFontSize() {
	if (!ImGui::GetIO().KeyCtrl)
		return;
	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		return;

	float wheel = ImGui::GetIO().MouseWheel;

	if (static_cast<int32_t>(wheel) == 0)
		return;

	if (style.fontSize < limit.maxFontSize) {
		if (wheel > 0)
			style.fontSize += wheel * 0.3f;
	}
	if (style.fontSize > limit.minFontSize) {
		if (wheel < 0)
			style.fontSize += wheel * 0.3f;
	}

	style.fontSizeScale = style.fontSize / ImGui::GetFontSize();
}








