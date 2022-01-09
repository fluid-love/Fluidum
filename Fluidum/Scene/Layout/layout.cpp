#include "layout.h"
#include "../Utils/Popup/message.h"

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
	FluidumScene_Log_Constructor(::FS::Layout);

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

	this->updateWindows();
}

FS::Layout::~Layout() noexcept {
	FluidumScene_Log_Destructor(::FS::Layout);
}

void FS::Layout::call() {
	this->noresize();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImGui::GetStyle().WindowPadding / 6.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.007f, 0.000f, 0.000f, 1.000f));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4());
	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4());
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4());
	ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4());

	this->dockGui();

	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(5);

	this->popup();
	this->save_resize();
	this->drawSeparators();
}

void FS::Layout::dockGui() {

	for (UIF16 i = 0, size = static_cast<UIF16>(windows.size()); i < size; i++) {
		select.resizedWindowIndex = i;
		select.current = &windows[i];
		std::string label = "##Lay" + std::to_string(i);

		this->dockSpace(label.c_str());

	}

}

void FS::Layout::dockSpace(const char* label) {
	constexpr auto windowFlags =
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowSizeConstraints(select.current->minSize, select.current->maxSize);
	ImGui::SetNextWindowPos(select.current->pos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(select.current->size, ImGuiCond_Always);


	ImGui::Begin(label, nullptr, windowFlags | (flag.noresize ? ImGuiWindowFlags_NoResize : ImGuiWindowFlags_None));

	select.current->pos = ImGui::GetWindowPos();
	select.current->size = ImGui::GetWindowSize();
	this->ifRightMouseButtonCliked();

	auto id = ImGui::GetID(label);

	ImGuiID dockingID = ImGui::DockSpace(id, ImVec2{}, ImGuiDockNodeFlags_PassthruCentralNode);

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowFocused()) {
		flag.mouseDown = true;
		const auto border = layoutWrite->update(*select.current, ImGui::GetMousePos(), select.resizeBorder);
		if (select.resizeBorder == FD::Layout::ResizedBorder::None)
			select.resizeBorder = border;
		this->updateWindows();
	}

	this->focusedWindowBackground();

	ImGui::End();
}

void FS::Layout::ifRightMouseButtonCliked() {
	if (!ImGui::IsMouseHoveringRect(select.current->pos, select.current->pos + select.current->size))
		return;

	select.hovered = select.current;
	select.focused = false;

	if (
		ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) ||
		ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		return;
	}

	select.focused = true;

	if (!ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		return;



	flag.popup = true;
	select.pos = ImGui::GetMousePos();
	select.right = select.current;

	flag.widthConstraintArea = this->widthConstraintArea();
	flag.heightConstraintArea = this->heightConstraintArea();

	flag.centerHorizonalConstraintArea = this->centerHorizonalConstraintArea();
	flag.centerVerticalConstraintArea = this->centerVerticalConstraintArea();

	flag.canMerge = layoutRead->canMerge(*select.right, select.pos);

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

	if (ImGui::MenuItem(text.merge, 0, false, flag.canMerge && !(flag.widthConstraintArea && flag.heightConstraintArea) && (flag.widthConstraintArea || flag.heightConstraintArea)))
		this->merge();

	ImGui::PopStyleVar();

	ImGui::EndPopup();
}

bool FS::Layout::widthConstraintArea() {
	const float width = layoutRead->widthLimit();
	for (auto& x : windows) {
		if (x.pos.y > select.pos.y || x.pos.y + x.size.y < select.pos.y)
			continue;

		if (x.pos.x - width < select.pos.x && select.pos.x < x.pos.x + width)
			return true;
		if (x.pos.x + x.size.x - width < select.pos.x && select.pos.x < x.pos.x + x.size.x + width)
			return true;
	}

	return false;
}

bool FS::Layout::heightConstraintArea() {
	const float height = layoutRead->heightLimit();
	for (auto& x : windows) {
		if (x.pos.x > select.pos.x || x.pos.x + x.size.x < select.pos.x)
			continue;

		if (x.pos.y - height < select.pos.y && select.pos.y < x.pos.y + height)
			return true;
		if (x.pos.y + x.size.y - height < select.pos.y && select.pos.y < x.pos.y + x.size.y + height)
			return true;
	}

	return false;
}

bool FS::Layout::centerHorizonalConstraintArea() {
	const float height = layoutRead->heightLimit();
	const float center = select.right->pos.y + (select.right->size.y / 2.0f);
	for (auto& x : windows) {
		if (x.pos.x > select.pos.x || x.pos.x + x.size.x < select.pos.x)
			continue;

		if (x.pos.y - height < center && center < x.pos.y + height)
			return true;
	}
	return false;
}

bool FS::Layout::centerVerticalConstraintArea() {
	const float width = layoutRead->widthLimit();
	const float center = select.right->pos.x + (select.right->size.x / 2.0f);
	for (auto& x : windows) {
		if (x.pos.y > select.pos.y || x.pos.y + x.size.y < select.pos.y)
			continue;

		if (x.pos.x - width < center && center < x.pos.x + width)
			return true;
	}
	return false;
}

void FS::Layout::splitVerticalCurrentPos() {
	const bool res = layoutWrite->splitVertical(*select.right, select.pos.x);

	if (!res) {
		this->messageLimit();
		return;
	}

	layoutWrite->save();
	this->updateWindows();
}

void FS::Layout::splitHorizonalCurrentPos() {
	const bool res = layoutWrite->splitHorizonal(*select.right, select.pos.y);

	if (!res) {
		this->messageLimit();
		return;
	}

	layoutWrite->save();
	this->updateWindows();
}

void FS::Layout::splitVerticalCenterLine() {
	const bool res = layoutWrite->splitVertical(*select.right, select.right->pos.x + (select.right->size.x / 2.0f));

	if (!res) {
		this->messageLimit();
		return;
	}

	layoutWrite->save();
	this->updateWindows();
}

void FS::Layout::splitHorizonalCenterLine() {
	const bool res = layoutWrite->splitHorizonal(*select.right, select.right->pos.y + (select.right->size.y / 2.0f));

	if (!res) {
		this->messageLimit();
		return;
	}

	layoutWrite->save();
	this->updateWindows();
}

void FS::Layout::reset() {
	const auto clicked = FU::MB::ok_cancel(text.confirm_reset);
	if (clicked == 0) //ok
		;
	else { //cancel
		assert(clicked == 1);
		return;
	}
	GLog.add<FU::Log::Type::None>(__FILE__, __LINE__, "Reset layout.");
	layoutWrite->reset();
	layoutWrite->save();
	this->updateWindows();
}

void FS::Layout::merge() {
	layoutWrite->merge(*select.right, select.pos);
	layoutWrite->save();
	this->updateWindows();
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

	if (!select.hovered)
		return;

	const float rectMaxX = select.hovered->pos.x + select.hovered->size.x;
	const float rectMaxY = select.hovered->pos.y + select.hovered->size.y;

	if (!(mousePos.y > rectMaxY - 17.0f && mousePos.y < rectMaxY))
		return;

	if (
		(mousePos.x > select.hovered->pos.x && mousePos.x < select.hovered->pos.x + 17.0f) ||
		(mousePos.x > rectMaxX - 17.0f && mousePos.x < rectMaxX)
		)
	{
		this->flag.noresize = true;
	}

}

void FS::Layout::save_resize() {
	if (!flag.mouseDown)
		return;

	if (select.resizeBorder == FD::Layout::ResizedBorder::None)
		return;

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
		return;

	layoutWrite->resize(windows[select.resizedWindowIndex], select.resizeBorder);
	layoutWrite->save();

	select.resizeBorder = FD::Layout::ResizedBorder::None;
	flag.mouseDown = false;
}

void FS::Layout::updateWindows() {
	select.current = nullptr;
	select.hovered = nullptr;
	select.right = nullptr;
	this->windows = layoutRead->get();
	this->separators = layoutRead->getSeparators();
}

void FS::Layout::messageLimit() {
	FluidumScene_Log_RequestAddScene(::FS::Utils::Message);
	Scene::addScene<Utils::Message>(text.error_max, select.pos);
}

void FS::Layout::focusedWindowBackground() {
	if (select.focused) {
		const ImVec2 padding = ImGui::GetStyle().WindowPadding;
		const ImVec2 pos = select.current->pos;
		const ImVec2 size = select.current->size;
		constexpr ImU32 col = FU::ImGui::ConvertImVec4ToImU32(0.00f, 0.004f, 0.004f, 1.0f);
		ImGui::GetWindowDrawList()->AddRectFilled(pos + padding, pos + size - padding, col);
	}
}

void FS::Layout::drawSeparators() {
	ImDrawList* list = ImGui::GetBackgroundDrawList();
	constexpr ImU32 col = FU::ImGui::ConvertImVec4ToImU32(0.366f, 0.366f, 0.366f, 1.000f);
	constexpr ImU32 colResize = FU::ImGui::ConvertImVec4ToImU32(0.3f, 1.0f, 0.4f, 1.0f);

	for (auto& x : separators) {
		if (x.resize)
			list->AddLine(x.pos1, x.pos2, select.resizeBorder != FD::Layout::ResizedBorder::None ? colResize : col);
		else {
			list->AddLine(x.pos1, x.pos2, col);
		}
	}
}
