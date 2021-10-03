#include "layout.h"

namespace FD {
	struct Gui final {
		ImVec2 centerPos{};
		ImVec2 windowSize{};
		float leftBarWidth = 0.0f;
		float menuBarHeight = 0.0f;
		float topBarHeight = 0.0f;
	};
	Gui GGui = {};

}


void FD::GuiWrite::centerPos(const ImVec2& vec2) const noexcept {
	GGui.centerPos = vec2;
}

void FD::GuiWrite::windowSize(const ImVec2& vec2) const noexcept {
	GGui.windowSize = vec2;
}

void FD::GuiWrite::leftBarWidth(const float width) const noexcept {
	GGui.leftBarWidth = width;
}

void FD::GuiWrite::menuBarHeight(const float height) const noexcept {
	GGui.menuBarHeight = height;
}

void FD::GuiWrite::topBarHeight(const float height) const noexcept {
	GGui.topBarHeight = height;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const ImVec2& FD::GuiRead::centerPos() const noexcept {
	return GGui.centerPos;
}

const ImVec2& FD::GuiRead::windowSize() const noexcept {
	return GGui.windowSize;
}

float FD::GuiRead::leftBarWidth() const noexcept {
	return GGui.leftBarWidth;
}

float FD::GuiRead::menuBarHeight() const noexcept {
	return GGui.menuBarHeight;
}

float FD::GuiRead::topBarHeight() const noexcept {
	return GGui.topBarHeight;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace FD {
	struct Layout final {
		ImVec2 leftLayoutPos;
		ImVec2 leftLayoutSize;
		ImVec2 rightLayoutPos;
		ImVec2 rightLayoutSize;
	};
	Layout GLayout = {};
}

void FD::LayoutWrite::leftLayoutPos(const ImVec2& vec2) const noexcept {
	GLayout.leftLayoutPos = vec2;
}

void FD::LayoutWrite::leftLayoutSize(const ImVec2& vec2) const noexcept {
	GLayout.leftLayoutSize = vec2;
}

void FD::LayoutWrite::rightLayoutPos(const ImVec2& vec2) const noexcept {
	GLayout.rightLayoutPos = vec2;
}

void FD::LayoutWrite::rightLayoutSize(const ImVec2& vec2) const noexcept {
	GLayout.rightLayoutSize = vec2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ImVec2 FD::LayoutRead::leftLayoutPos() const noexcept {
	return GLayout.leftLayoutPos;
}

ImVec2 FD::LayoutRead::leftLayoutSize() const noexcept {
	return GLayout.leftLayoutSize;
}

ImVec2 FD::LayoutRead::rightLayoutPos() const noexcept {
	return GLayout.rightLayoutPos;
}

ImVec2 FD::LayoutRead::rightLayoutSize() const noexcept {
	return GLayout.rightLayoutSize;
}