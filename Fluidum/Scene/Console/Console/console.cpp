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
	ImGui::Begin("Console", &this->windowFlag);

	this->console();
	this->input();


	this->changeFontSize();
	ImGui::End();

	this->closeWindow();
}

void FS::Console::console() {
	ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.005f,0.005f,0.005f,1.0f });

	const auto size = ImGui::GetWindowSize();
	ImGui::BeginChild("ConsoleText", { size.x * 0.98f,size.y * 0.8f }, false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::SetWindowFontScale(style.fontSizeScale);

	ImGuiListClipper clipper;
	clipper.Begin(100);
	while (clipper.Step()) {

		for (int32_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {

			const auto info = consoleRead->get(i);
			if (info.first)
				ImGui::Text(info.second.message.c_str());
			else
				ImGui::Spacing();
		}
	}

	ImGui::EndChild();

	ImGui::PopStyleColor();
}

void FS::Console::input() {
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.8f);
	ImGui::InputText("##InputText", inputText.data(), inputText.capacity());

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) && ImGui::IsItemFocused()) {
		this->push();
	}

	ImGui::SameLine();

	if (ImGui::Button(ICON_MD_ADD)) {
		this->push();
	}
}

void FS::Console::closeWindow() {
	if (windowFlag)
		return;

	//close
	FluidumScene_Log_RequestDeleteScene("Console");
	Scene::deleteScene<Console>();
}

void FS::Console::push() {
	consoleWrite->add(std::string(inputText.data()));
	inputText = std::string();
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








