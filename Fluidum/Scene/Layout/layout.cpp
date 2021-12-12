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
		const ImVec2 pos = { guiRead->leftBarWidth() ,guiRead->menuBarHeight() + guiRead->topBarHeight() };
		layoutWrite->mainFrameLeft(pos.x);
		layoutWrite->mainFrameRight(guiRead->windowSize().x);
		layoutWrite->mainFrameTop(pos.y);
		layoutWrite->mainFrameBottom(guiRead->windowSize().y - guiRead->statusBarHeight());
		layoutWrite->reset();
	}

	layoutWrite->widthLimit(guiRead->windowSize().x * 0.13f);
	layoutWrite->heightLimit(guiRead->windowSize().y * 0.13f);

}

FS::Layout::~Layout() noexcept {
	FluidumScene_Log_Destructor_("LeftLayuot")
}

void FS::Layout::call() {
	this->noresize();

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
	this->windows = layoutRead->get();
	for (uint16_t i = 0, size = static_cast<uint16_t>(windows.size()); i < size; i++) {
		select.current = windows[i];
		std::string label = "##Lay" + std::to_string(i);

		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, { 0.007f,0.0f ,0.0f ,1.0f });
		this->dockSpace(label.c_str());
		ImGui::PopStyleColor();
	}
}

void FS::Layout::dockSpace(const char* label) {
	constexpr auto windowFlags =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings;

	ImGui::SetNextWindowSizeConstraints(select.current.minSize, select.current.maxSize);
	ImGui::SetNextWindowPos(select.current.pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(select.current.size, ImGuiCond_Always);

	ImGui::Begin(label, nullptr, windowFlags | (flag.noresize ? ImGuiWindowFlags_NoResize : ImGuiWindowFlags_None));

	auto id = ImGui::GetID(label);
	ImGuiID dockingID = ImGui::DockSpace(id, ImVec2{});

	select.current.pos = ImGui::GetWindowPos();
	select.current.size = ImGui::GetWindowSize();

	this->ifRightMouseButtonCliked();

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowFocused())
		layoutWrite->update(select.current);

	ImGui::End();
}

void FS::Layout::ifRightMouseButtonCliked() {
	if (!ImGui::IsMouseClicked(ImGuiMouseButton_Right) || !ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_RootAndChildWindows))
		return;

	flag.popup = true;
	select.pos = ImGui::GetMousePos();
	select.right = select.current;

	flag.widthConstraintArea = this->widthConstraintArea();
	flag.heightConstraintArea = this->heightConstraintArea();

	flag.centerHorizonalConstraintArea = this->centerHorizonalConstraintArea();
	flag.centerVerticalConstraintArea = this->centerVerticalConstraintArea();

}

void FS::Layout::popup() {
	if (flag.popup) {
		ImGui::OpenPopup("Layout");
		flag.popup = false;
	}

	if (!ImGui::BeginPopup("Layout"))
		return;


	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x,ImGui::GetStyle().ItemSpacing.y * 2.2f });

	if (ImGui::MenuItem(text.splitVerticalCurrentPos, 0, false, !flag.widthConstraintArea))
		this->splitVerticalCurrentPos();
	if (ImGui::MenuItem(text.splitHorizonalCurrentPos, 0, false, !flag.heightConstraintArea))
		this->splitHorizonalCurrentPos();

	ImGui::Separator();

	if (ImGui::MenuItem(text.splitVerticalCenterLine, 0, false, !flag.centerVerticalConstraintArea))
		this->splitVerticalCenterLine();
	if (ImGui::MenuItem(text.splitHorizonalCenterLine, 0, false, !flag.centerHorizonalConstraintArea))
		this->splitHorizonalCenterLine();


	ImGui::Separator();

	if (ImGui::Selectable(text.reset))
		this->reset();

	ImGui::Separator();

	if (ImGui::MenuItem(text.merge, 0, false, flag.widthConstraintArea))
		this->merge();

	ImGui::PopStyleVar();

	ImGui::EndPopup();
}

bool FS::Layout::widthConstraintArea() {
	const float width = layoutRead->widthLimit();
	for (auto& x : windows) {
		if (x.pos.x - width < select.pos.x && select.pos.x < x.pos.x + width)
			return true;
	}

	return false;
}

bool FS::Layout::heightConstraintArea() {
	const float height = layoutRead->heightLimit();
	for (auto& x : windows) {
		if (x.pos.y - height < select.pos.y && select.pos.y < x.pos.y + height)
			return true;
	}

	return false;
}

bool FS::Layout::centerHorizonalConstraintArea() {
	const float height = layoutRead->heightLimit();
	const float center = select.right.pos.y + (select.right.size.y / 2.0f);
	for (auto& x : windows) {
		if (x.pos.y - height < center && center < x.pos.y + height)
			return true;
	}
	return false;
}

bool FS::Layout::centerVerticalConstraintArea() {
	const float width = layoutRead->widthLimit();
	const float center = select.right.pos.x + (select.right.size.x / 2.0f);
	for (auto& x : windows) {
		if (x.pos.x - width < center && center < x.pos.x + width)
			return true;
	}
	return false;
}

void FS::Layout::splitVerticalCurrentPos() {
	layoutWrite->splitVertical(select.right, select.pos.x);
	layoutWrite->save();
}

void FS::Layout::splitHorizonalCurrentPos() {
	layoutWrite->splitHorizonal(select.right, select.pos.y);
	layoutWrite->save();
}

void FS::Layout::splitVerticalCenterLine() {
	layoutWrite->splitVertical(select.right, select.right.pos.x + (select.right.size.x / 2.0f));
	layoutWrite->save();
}

void FS::Layout::splitHorizonalCenterLine() {
	layoutWrite->splitHorizonal(select.right, select.right.pos.y + (select.right.size.y / 2.0f));
	layoutWrite->save();
}

void FS::Layout::reset() {
	const auto clicked = FU::MB::ok_cancel("");
	if (clicked == 0) //ok
		;
	else { //cancel
		assert(clicked == 1);
		return;
	}
	GLog.add<FD::Log::Type::None>("Reset layout.");
	layoutWrite->reset();
	layoutWrite->save();
}

void FS::Layout::merge() {
	layoutWrite->save();
}

void FS::Layout::noresize() {
	const ImVec2 mousePos = ImGui::GetMousePos();
	const float left = layoutRead->mainFrameLeft();
	const float right = layoutRead->mainFrameRight();
	const float top = layoutRead->mainFrameTop();
	const float bottom = layoutRead->mainFrameBottom();

	if (
		(left - 8.0f < mousePos.x && left + 8.0f > mousePos.x) ||
		(right - 8.0f < mousePos.x && right + 8.0f > mousePos.x) ||
		(top - 8.0f < mousePos.y && top + 8.0f > mousePos.y) ||
		(bottom - 8.0f < mousePos.y && bottom + 8.0f > mousePos.y)
		)
	{
		this->flag.noresize = true;
	}
	else
		this->flag.noresize = false;
}
