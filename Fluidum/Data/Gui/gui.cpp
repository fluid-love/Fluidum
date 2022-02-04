#include "gui.h"

namespace FD {
	struct Gui final {
		ImVec2 windowPos{};
		ImVec2 centerPos{};
		ImVec2 windowSize{};
		float leftBarWidth = 0.0f;
		float menuBarHeight = 0.0f;
		float topBarHeight = 0.0f;
		float statusBarHeight = 0.0f;
		float titleBarLeft{};
		float windowLimitMinWidth = 0.0f;
		float windowLimitMinHeight = 0.0f;
	};
	Gui GGui = {};
}

void FD::GuiWrite::windowPos(const ImVec2& vec2) noexcept {
	GGui.windowPos = vec2;
}

void FD::GuiWrite::centerPos(const ImVec2& vec2) noexcept {
	GGui.centerPos = vec2;
}

void FD::GuiWrite::windowSize(const ImVec2& vec2) noexcept {
	GGui.windowSize = vec2;
}

void FD::GuiWrite::leftBarWidth(const float width) noexcept {
	GGui.leftBarWidth = width;
}

void FD::GuiWrite::menuBarHeight(const float height) noexcept {
	GGui.menuBarHeight = height;
}

void FD::GuiWrite::topBarHeight(const float height) noexcept {
	GGui.topBarHeight = height;
}

void FD::GuiWrite::statusBarHeight(const float height) noexcept {
	GGui.statusBarHeight = height;
}

void FD::GuiWrite::titleBarLeft(const float left) noexcept {
	GGui.titleBarLeft = left;
}

void FD::GuiWrite::windowLimitMinWidth(const float width) noexcept {
	GGui.windowLimitMinWidth = width;
}

void FD::GuiWrite::windowLimitMinHeight(const float height) noexcept {
	GGui.windowLimitMinHeight = height;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const ImVec2& FD::GuiRead::windowPos() const noexcept {
	return GGui.windowPos;
}

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

float FD::GuiRead::statusBarHeight() const noexcept {
	return GGui.statusBarHeight;
}

float FD::GuiRead::titleBarLeft() const noexcept {
	return GGui.titleBarLeft;
}

float FD::GuiRead::windowLimitMinWidth() const noexcept {
	return GGui.windowLimitMinWidth;
}

float FD::GuiRead::windowLimitMinHeight() const noexcept {
	return GGui.windowLimitMinHeight;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <imgui_internal.h>

namespace FD {
	std::map<FU::Class::ClassCode::CodeType, ImGuiWindow*> GImGuiWindows{};
}

template<typename T>
void FD::ImGuiWindowWrite::set(const FU::Class::ClassCode::CodeType classCode, T windowPtr) const {
	if (GImGuiWindows.contains(classCode))
		GImGuiWindows.at(classCode) = windowPtr;
	else
		GImGuiWindows.insert({ classCode,windowPtr });
}
template void FD::ImGuiWindowWrite::set<ImGuiWindow*>(const FU::Class::ClassCode::CodeType, ImGuiWindow*) const;

template<typename T>
T FD::ImGuiWindowRead::get(const FU::Class::ClassCode::CodeType classCode) const {
	return GImGuiWindows.at(classCode);
}
template ImGuiWindow* FD::ImGuiWindowRead::get<ImGuiWindow*>(const FU::Class::ClassCode::CodeType) const;