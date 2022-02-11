#include "window_resize.h"

FS::Misc::ResizeWindow::ResizeWindow(
	const FD::GuiRead* const guiRead,
	FD::GuiWrite* const guiWrite,
	const FD::Style::ColorRead* const colorRead
) :
	guiRead(guiRead),
	guiWrite(guiWrite),
	colorRead(colorRead)
{
	FluidumScene_Log_Constructor(::FS::Misc::ResizeWindow);
}

FS::Misc::ResizeWindow::~ResizeWindow() noexcept {
	FluidumScene_Log_Destructor(::FS::Misc::ResizeWindow);
}

void FS::Misc::ResizeWindow::call() {
	this->background();

	using namespace FU::ImGui::Operators;

	if (!FDR::isWindowFocused())
		return;

	//update
	{
		const auto mPos = ImGui::GetMousePos();
		const auto pos = FDR::getWindowPos();
		const auto size = FDR::getWindowSize();
		const auto dragDelta = ImGui::GetMouseDragDelta();
		static ImVec2 old = mPos;
		const auto delta = mPos - old;
		old = mPos;
		mousePosX = static_cast<IF32>(mPos.x);
		mousePosY = static_cast<IF32>(mPos.y);
		windowPosX = pos.first;
		windowPosY = pos.second;
		windowSizeX = size.first;
		windowSizeY = size.second;
		deltaX = static_cast<IF32>(delta.x);
		deltaY = static_cast<IF32>(delta.y);
		dragDeltaX = static_cast<IF32>(dragDelta.x);
		dragDeltaY = static_cast<IF32>(dragDelta.y);
		down = ImGui::IsMouseDown(ImGuiMouseButton_Left);

		ImGui::GetMainViewport()->Pos = viewPortPos;
	}

	this->moveWindow();
	if (FDR::isWindowFullscreen()) 
		return;

	this->decorateWindow();

	if (ImGui::IsAnyItemHovered())
		return;

	const float innerX = windowSizeX * 0.0067f;
	const float innerY = windowSizeY * 0.0067f;
	const float outerX = 0.0f;//windowSizeX * 0.0248f;
	const float outerY = 0.0f;//windowSizeY * 0.0048f;

	bool hovered = false;
	bool clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);

	//left
	if (
		mousePosX < innerX &&
		mousePosX > -outerX &&
		mousePosY > innerY &&
		mousePosY < (windowSizeY - innerY)
		)
	{
		currentBorder = Border::Left;
		if (clicked && holdBorder == Border::None)
			holdBorder = Border::Left;
		hovered = true;
	}

	//right
	else if (
		mousePosX < (windowSizeX + outerX) &&
		mousePosX >(windowSizeX - innerX) &&
		mousePosY > innerY &&
		mousePosY < (windowSizeY - innerY)
		)
	{
		currentBorder = Border::Right;
		if (clicked && holdBorder == Border::None)
			holdBorder = Border::Right;
		hovered = true;
	}

	//top
	else if (
		mousePosY < innerY &&
		mousePosY > -outerY &&
		mousePosX > innerX &&
		mousePosX < (windowSizeX - innerX)
		)
	{
		currentBorder = Border::Top;
		if (clicked && holdBorder == Border::None)
			holdBorder = Border::Top;
		hovered = true;
	}

	//bottom
	else if (
		mousePosY < (windowSizeY + outerY) &&
		mousePosY >(windowSizeY - innerY) &&
		mousePosX > innerX &&
		mousePosX < (windowSizeX - innerX)
		)
	{
		currentBorder = Border::Bottom;
		if (clicked && holdBorder == Border::None)
			holdBorder = Border::Bottom;
		hovered = true;
	}

	if (!hovered)
		this->currentBorder = Border::None;

	if (holdBorder == Border::None && down)
		holdBorder = currentBorder;

	this->hold();
	this->updateGuiData();
}

void FS::Misc::ResizeWindow::background() {
	//if (holdBorder == Border::None || holdBorder == Border::Move)
	//	return;

	auto list = ImGui::GetBackgroundDrawList();

	list->AddRectFilled(
		{ -50.0f, -50.0f },
		{ static_cast<float>(windowSizeX), static_cast<float>(windowSizeY) },
		IM_COL32_BLACK
	);
}

void FS::Misc::ResizeWindow::leftCursor() {
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	FU::Cursor::setCursorType(FU::Cursor::Type::SizeWE);
}

void FS::Misc::ResizeWindow::rightCursor() {
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	FU::Cursor::setCursorType(FU::Cursor::Type::SizeWE);
}

void FS::Misc::ResizeWindow::topCursor() {
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	FU::Cursor::setCursorType(FU::Cursor::Type::SizeNS);
}

void FS::Misc::ResizeWindow::bottomCursor() {
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	FU::Cursor::setCursorType(FU::Cursor::Type::SizeNS);
}

void FS::Misc::ResizeWindow::resize_left() {
	assert(holdBorder == Border::Left);

	if (!down)
		return;

	if (dragDeltaX <= 0 && windowSizeX >= FDR::getFullscreenWindowSize().first) {
		limit = Border::Left;
		return;
	}
	this->setLimitNone();

	if (dragDeltaX >= 0 && windowSizeX <= guiRead->windowLimitMinWidth())
		return;

	FDR::setWindowPosSize_timing(
		windowPosX + dragDeltaX,
		windowPosY,
		windowSizeX - dragDeltaX,
		windowSizeY
	);
}

void FS::Misc::ResizeWindow::resize_right() {
	assert(holdBorder == Border::Right);

	if (!down)
		return;

	if (deltaX >= 0 && windowSizeX >= FDR::getFullscreenWindowSize().first) {
		limit = Border::Right;
		return;
	}
	this->setLimitNone();

	if (deltaX <= 0 && windowSizeX <= guiRead->windowLimitMinWidth())
		return;

	FDR::setWindowSize_timing(windowSizeX + deltaX, windowSizeY);
}

void FS::Misc::ResizeWindow::resize_top() {
	assert(holdBorder == Border::Top);

	if (!down)
		return;

	if (dragDeltaY <= 0 && windowSizeY >= FDR::getFullscreenWindowSize().second) {
		limit = Border::Top;
		return;
	}
	this->setLimitNone();

	if (dragDeltaY >= 0 && windowSizeY <= guiRead->windowLimitMinHeight())
		return;

	FDR::setWindowPosSize_timing(
		windowPosX,
		windowPosY + dragDeltaY,
		windowSizeX,
		windowSizeY - dragDeltaY
	);
}

void FS::Misc::ResizeWindow::resize_bottom() {
	assert(holdBorder == Border::Bottom);

	if (!down)
		return;

	if (deltaY >= 0 && windowSizeY >= FDR::getFullscreenWindowSize().second) {
		limit = Border::Bottom;
		return;
	}
	this->setLimitNone();

	if (deltaY <= 0 && windowSizeY <= guiRead->windowLimitMinHeight())
		return;

	FDR::setWindowSize_timing(windowSizeX, windowPosY + deltaY);
}

void FS::Misc::ResizeWindow::hold() {

	bool hovered = false;

	//cursor
	if (currentBorder == Border::Left) {
		this->leftCursor();
		hovered = true;
	}
	else if (currentBorder == Border::Right) {
		this->rightCursor();
		hovered = true;
	}
	else if (currentBorder == Border::Top) {
		this->topCursor();
		hovered = true;
	}
	else if (currentBorder == Border::Bottom) {
		this->bottomCursor();
		hovered = true;
	}

	if (holdBorder == Border::Left) {
		this->resize_left();
		if (!hovered)
			this->leftCursor();
	}
	else if (holdBorder == Border::Right) {
		this->resize_right();
		if (!hovered)
			this->rightCursor();
	}
	else if (holdBorder == Border::Top) {
		this->resize_top();
		if (!hovered)
			this->topCursor();
	}
	else if (holdBorder == Border::Bottom) {
		this->resize_bottom();
		if (!hovered)
			this->bottomCursor();
	}

	if (!down) {
		holdBorder = Border::None;
		return;
	}
}

void FS::Misc::ResizeWindow::moveWindow() {
	if (holdBorder != Border::Move && mousePosY > guiRead->menuBarHeight())
		return;

	if (currentBorder == Border::Top || holdBorder == Border::Top)
		return;

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		this->isItemHoved = ImGui::IsAnyItemHovered();

	currentBorder = Border::Move;

	if (!down)
		return;

	if (isItemHoved)
		return;

	if (ImGui::IsAnyItemHovered())
		return;

	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	FU::Cursor::setCursorType(FU::Cursor::Type::SizeAll);
	holdBorder = Border::Move;

	auto [x, y] = FDR::getWindowPos();
	x += dragDeltaX;
	y += dragDeltaY;

	FDR::setWindowPos_timing(x, y);

}

void FS::Misc::ResizeWindow::decorateWindow() {
	this->decorateWindow_frame();
	this->decorateWindow_limitBorder();
}

void FS::Misc::ResizeWindow::decorateWindow_frame() {
	auto list = ImGui::GetForegroundDrawList();

	if (holdBorder == Border::Move) {
		list->AddRect(
			{ 0.0f, 0.0f },
			{ static_cast<float>(windowSizeX) , static_cast<float>(windowSizeY) },
			ImGui::ColorConvertFloat4ToU32(colorRead->noerror()),
			0.0f,
			0,
			1.85f
		);
	}
	else if (holdBorder == Border::None) {
		list->AddRect(
			{ 0.0f, 0.0f },
			{ static_cast<float>(windowSizeX) , static_cast<float>(windowSizeY) },
			colorRead->windowBorder(),
			0.0f,
			0,
			1.85f
		);
	}

}

void FS::Misc::ResizeWindow::decorateWindow_limitBorder() {
	if (limit == Border::None)
		return;

	if (holdBorder == Border::None) {
		return;
	}

	if (limit != holdBorder)
		return;

	auto list = ImGui::GetForegroundDrawList();

	const auto [pos1, pos2] = this->borderPos(limit);

	list->AddLine(
		pos1,
		pos2,
		ImGui::ColorConvertFloat4ToU32(colorRead->error()),
		2.05f
	);

}

void FS::Misc::ResizeWindow::setLimitNone() {
	this->limit = Border::None;
}

std::pair<ImVec2, ImVec2> FS::Misc::ResizeWindow::borderPos(const Border border) {
	ImVec2 pos1 = { 0.0f, 0.0f };
	ImVec2 pos2 = { static_cast<float>(windowSizeX), static_cast<float>(windowSizeY) };

	if (holdBorder == Border::Left) {
		pos2.x = 0.0f;
	}
	else if (holdBorder == Border::Right) {
		pos1.x = pos2.x - 1.0f;
	}
	else if (holdBorder == Border::Top) {
		pos2.y = 0.0f;
	}
	else {
		pos1.y = pos2.y;
	}

	return { pos1, pos2 };
}

void FS::Misc::ResizeWindow::updateGuiData() {
	guiWrite->windowSize({ static_cast<float>(windowSizeX), static_cast<float>(windowSizeY) });
}
