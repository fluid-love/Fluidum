#include "layout.h"

using namespace FU::ImGui::Operators;

FS::Layout::Layout(
	const FD::LayoutRead* const layoutRead,
	FD::LayoutWrite* const layoutWrite,
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite
) :
	layoutRead(layoutRead),
	layoutWrite(layoutWrite),
	guiRead(guiRead),
	guiWrite(guiWrite)
{
	FluidumScene_Log_Constructor("LeftLayuot");

	//default project
	if (layoutRead->empty()) {
		FD::Layout::DockSpaceWindow window{};
		window.pos = { guiRead->leftBarWidth() ,guiRead->menuBarHeight() + guiRead->topBarHeight() - 4.0f };
		window.size = { guiRead->windowSize().x - guiRead->leftBarWidth(),guiRead->windowSize().y - window.pos.y - guiRead->statusBarHeight() + 4.0f };
		layoutWrite->push(window);
		layoutWrite->leftPos(window.pos);
		layoutWrite->rightPos({ window.pos.x + window.size.x ,window.pos.y });
	}

}

FS::Layout::~Layout() noexcept {
	FluidumScene_Log_Destructor_("LeftLayuot")
}

void FS::Layout::call() {

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.006f, 0.005f, 0.005f, 1.000f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.266f, 0.200f, 0.200f, 1.000f));

	this->dockGui();

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);

	this->popup();
}

void FS::Layout::dockGui() {

	for (uint16_t i = 0, size = layoutRead->size(); i < size; i++) {
		select.current = layoutRead->get(i);
		std::string label = "##Lay" + std::to_string(i);
		this->dockSpace(label.c_str());
	}


	//if (!flag.limitLeft)
	//	ImGui::Begin("LeftLayout", nullptr, windowFlag);
	//else
	//	ImGui::Begin("LeftLayout", nullptr, windowFlag | ImGuiWindowFlags_NoResize);

	//id = ImGui::GetID("LeftLayout");

	//ImGui::Dummy({ 0.0f,0.8f });
	//ImGuiID dockingID = ImGui::DockSpace(id, ImVec2{});
	//layoutWrite->leftDockSpaceID(dockingID);

	//auto currentSizeX = ImGui::GetWindowSize().x;
	//if (static_cast<int32_t>(currentSizeX) != static_cast<int32_t>(style.windowSize.x)) {
	//	style.windowSize.x = currentSizeX;
	//	flag.isChanged = true;
	//}

	//if (style.windowSize.x > guiRead->windowSize().x * 0.8f) {
	//	style.windowSize.x = guiRead->windowSize().x * 0.8f;
	//	flag.limitLeft = true;
	//}
	//else if (style.windowSize.x < guiRead->windowSize().x * 0.15f) {
	//	style.windowSize.x = guiRead->windowSize().x * 0.15f;
	//	flag.limitLeft = true;
	//}
	//else {
	//	flag.limitLeft = false;
	//}

	//layoutWrite->leftLayoutSize(style.windowSize);


	//ImGui::End();
}

void FS::Layout::dockSpace(const char* label) {
	constexpr auto windowFlags =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar;

	ImGui::SetNextWindowPos(select.current.pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(select.current.size, ImGuiCond_Always);

	ImGui::Begin(label, nullptr, 0);

	auto id = ImGui::GetID(label);
	ImGuiID dockingID = ImGui::DockSpace(id, ImVec2{});

	select.current.pos = ImGui::GetWindowPos();
	select.current.size = ImGui::GetWindowSize();

	this->ifRightMouseButtonCliked();
	ImGui::End();

	//pos size
	layoutWrite->update(select.current);
}

void FS::Layout::ifRightMouseButtonCliked() {
	if (!ImGui::IsMouseClicked(ImGuiMouseButton_Right) || !ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
		return;

	flag.popup = true;

	select.pos = ImGui::GetMousePos();
	select.right = select.current;
}

void FS::Layout::popup() {
	if (flag.popup) {
		ImGui::OpenPopup("Layout");
		flag.popup = false;
	}

	if (!ImGui::BeginPopup("Layout"))
		return;


	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x,ImGui::GetStyle().ItemSpacing.y * 2.2f });

	if (ImGui::Selectable(text.splitVerticalCurrentPos))
		this->splitVerticalCurrentPos();
	if (ImGui::Selectable(text.splitHorizonalCurrentPos))
		this->splitHorizonalCurrentPos();
	if (ImGui::Selectable(text.splitCrossCurrentPos))
		this->splitCrossCurrentPos();

	ImGui::Separator();

	if (ImGui::Selectable(text.splitVerticalCenterLine))
		this->splitVerticalCenterLine();
	if (ImGui::Selectable(text.splitHorizonalCenterLine))
		this->splitHorizonalCenterLine();
	if (ImGui::Selectable(text.splitCrossCenterLine))
		this->splitCrossCenterLine();

	ImGui::PopStyleVar();

	ImGui::EndPopup();
}

void FS::Layout::splitVerticalCurrentPos() {
	layoutWrite->splitVertical(select.right, select.pos.x);
}

void FS::Layout::splitHorizonalCurrentPos() {
	layoutWrite->splitHorizonal(select.right, select.pos.y);
}

void FS::Layout::splitCrossCurrentPos() {
	layoutWrite->splitCross(select.right, select.pos);
}

void FS::Layout::splitVerticalCenterLine() {
	layoutWrite->splitVertical(select.right, select.right.pos.x + (select.right.size.x / 2.0f));
}

void FS::Layout::splitHorizonalCenterLine() {
	layoutWrite->splitHorizonal(select.right, select.right.pos.y + (select.right.size.y / 2.0f));
}

void FS::Layout::splitCrossCenterLine() {
	layoutWrite->splitCross(select.right,
		{
			select.right.pos.x + (select.right.size.x / 2.0f),
			select.right.pos.y + (select.right.size.y / 2.0f)
		}
	);
}

void FS::Layout::merge() {

}






