#include "console.h"
#include <imgui_internal.h>

FS::Console::Console(
	FD::ImGuiWindowWrite* const imguiWindowWrite, FD::ConsoleWrite* const consoleWrite,
	const FD::ConsoleRead* const consoleRead,
	const FD::Style::ColorRead* const colorRead,
	const FD::Style::VarRead* const varRead,
	FD::ToolBarWrite* const toolBarWrite
) :
	imguiWindowWrite(imguiWindowWrite),
	consoleWrite(consoleWrite),
	consoleRead(consoleRead),
	colorRead(colorRead),
	varRead(varRead),
	toolBarWrite(toolBarWrite)
{
	FluidumScene_Log_Constructor(::FS::Console);

	if (inputText.max_size() <= FD::Console::Limits::Characters)
		inputText.reserve(inputText.max_size());
	else
		inputText.reserve(FD::Console::Limits::Characters);

	style.inputTextWindowHeight = varRead->inputTextHeight();

	toolBarWrite->add(&Console::toolBar, this, text.console.operator const std::string & ());

}

FS::Console::~Console() noexcept {
	toolBarWrite->remove<Console>();
	FluidumScene_Log_Destructor(::FS::Console);
}

void FS::Console::call() {

	ImGui::SetNextWindowContentSize(varRead->viewWindowSizeConstraints());

	//collapse
	if (flag.collapseWindow) {
		ImGui::SetNextWindowCollapsed(true);
		flag.collapseWindow = false;
	}

	ImGui::Begin(text.console, &flag.windowFlag);
	this->setImGuiWindow();

	flag.isWindowCollpsed = ImGui::IsWindowCollapsed();

	//popup: title bar
	if (FU::ImGui::isTitleBarClicked(ImGuiMouseButton_Right))
		flag.popupTitle = true;

	//main
	this->console();
	this->input();


	this->changeFontSize();

	ImGui::End();

	this->popupTitle();
	this->popupRight();
	this->closeWindow();
}

void FS::Console::setImGuiWindow() {
	imguiWindowWrite->set(FU::Class::ClassCode::GetClassCode<Console>(), ImGui::GetCurrentWindow());
}

void FS::Console::toolBar() {
	ImGui::Button("test");
}

void FS::Console::console() {
	using namespace FU::ImGui::Operators;

	ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.005f,0.005f,0.005f,1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.1f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImGui::GetStyle().WindowPadding * 2.0f);

	const auto size = ImGui::GetWindowSize();

	ImGui::BeginChild("ConsoleText", { size.x - ImGui::GetStyle().ScrollbarSize, size.y - style.inputTextWindowHeight }, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::SetWindowFontScale(style.fontSizeScale);

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !flag.popupTitle && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
		flag.popupRight = true;

	ImGuiListClipper clipper;
	const IF32 clipperSize = static_cast<IF32>(consoleRead->size() < 100 ? consoleRead->size() : 100);
	clipper.Begin(clipperSize);
	while (clipper.Step()) {
		for (IF32 i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {

			auto [valid, message] = consoleRead->get(i);
			if (valid) {//read_only
				const ImVec4 color = this->messageColor(message.type);
				ImGui::TextColored(color, message.message.c_str());
			}
			else {
				ImGui::Spacing();
			}
		}
	}

	if (!consoleRead->busy()) {
		const float alpha = std::fabs(std::sinf(static_cast<float>(2.0 * ImGui::GetTime())));
		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f,1.0f,1.0f,alpha });
		ImGui::Text(">");
		ImGui::PopStyleColor();
	}

	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

ImVec4 FS::Console::messageColor(const FU::Log::Type type) const {
	using enum FU::Log::Type;
	if (type == None)
		return ImGui::GetStyleColorVec4(ImGuiCol_Text);
	else if (type == Error)
		return colorRead->error();
	else if (type == Warning)
		return colorRead->warning();
	else
		return colorRead->info();
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
	if (flag.windowFlag)
		return;

	//close
	FluidumScene_Log_RequestDeleteScene(::FS::Console);
	Scene::deleteScene<Console>();
}

void FS::Console::push() {
	std::string text = ">" + std::string(inputText.data());
	consoleWrite->push(std::move(text));
	inputText = std::string();

	if (inputText.max_size() <= FD::Console::Limits::Characters)
		inputText.reserve(inputText.max_size());
	else
		inputText.reserve(FD::Console::Limits::Characters);

	consoleWrite->busy(true);
	for (Size i = 452; i < 589; i++) {

		consoleWrite->push(std::string("Generation:{}. Fitness:{}."), i, (float)(double)i*100  / (double)i + 2 * 2 * (double)i * i + 1 / 2.5f);

	}
}

void FS::Console::popupTitle() {
	if (flag.popupTitle) {
		flag.popupTitle = false;
		ImGui::OpenPopup("TitlePopup");
	}

	if (!ImGui::BeginPopup("TitlePopup"))
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x,ImGui::GetStyle().ItemSpacing.y * 2.2f });

	if (flag.isWindowCollpsed) { //already collapsed
		ImGui::TextDisabled(text_.collapseWindow.c_str());
	}
	else {
		if (ImGui::Selectable(text_.collapseWindow.c_str()))
			flag.collapseWindow = true;
	}

	ImGui::PopStyleVar();

	ImGui::EndPopup();
}

void FS::Console::popupRight() {
	if (flag.popupRight) {
		flag.popupRight = false;
		ImGui::OpenPopup("ConsolePopup");
	}

	if (!ImGui::BeginPopup("ConsolePopup"))
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x,ImGui::GetStyle().ItemSpacing.y * 2.2f });

	if (ImGui::Selectable(text.clear))
		this->popup_clear();

	//ImGui::Separator();

	//if (ImGui::MenuItem(text.backcolor))
	//	this->popup_backcolor();

	ImGui::PopStyleVar();

	ImGui::EndPopup();
}

void FS::Console::popup_clear() {
	consoleWrite->clear();
}

void FS::Console::popup_backcolor() {
	//ImGui::ColorPicker4();
}

void FS::Console::changeFontSize() {
	if (!ImGui::GetIO().KeyCtrl)
		return;
	if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
		return;

	const float wheel = ImGui::GetIO().MouseWheel;

	if (static_cast<IF32>(wheel) == 0)
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

