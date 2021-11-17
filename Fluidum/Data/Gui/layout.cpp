#include "layout.h"
#include "../Limits/limits.h"
// number of divisions
namespace FD {
	struct Gui final {
		ImVec2 centerPos{};
		ImVec2 windowSize{};
		float leftBarWidth = 0.0f;
		float menuBarHeight = 0.0f;
		float topBarHeight = 0.0f;
		float statusBarHeight = 0.0f;
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

void FD::GuiWrite::statusBarHeight(const float height) const noexcept {
	GGui.statusBarHeight = height;
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

float FD::GuiRead::statusBarHeight() const noexcept {
	return GGui.statusBarHeight;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace FD::Layout {
	struct AdjacentInfo final {
		enum class Type : uint8_t {
			Left,
			Right,
			Top,
			Bottom
		}type;
		bool most = false;
		DockSpaceWindow* window{};
	};
	std::map<DockSpaceWindow*, std::vector<AdjacentInfo>> GAdjacent{};

	[[nodiscard]] ImVec2 getRightPos(DockSpaceWindow* identifier) {
		auto& vec = GAdjacent.at(identifier);
		for (auto& x : vec) {
			if (x.type == AdjacentInfo::Type::Right)
				return x.window->pos;
		}
		assert(false);
		ImVec2{};
	}
}

void FD::LayoutWrite::leftPos(const ImVec2& vec2) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	LayoutData::leftPos = vec2;
}

void FD::LayoutWrite::rightPos(const ImVec2& vec2) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	LayoutData::rightPos = vec2;
}

void FD::LayoutWrite::push(Layout::DockSpaceWindow& window) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	assert(LayoutData::windows.empty());

	std::shared_ptr<Layout::DockSpaceWindow> ptr = std::make_shared<Layout::DockSpaceWindow>(window);
	window.identifier = ptr.get();
	LayoutData::windows.emplace_back(std::move(ptr));
	LayoutData::windows.back()->identifier = LayoutData::windows.back().get();

	Layout::GAdjacent.insert({ LayoutData::windows.back().get(), {} });
}

void FD::LayoutWrite::splitVertical(const Layout::DockSpaceWindow& window, const float posX) {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	auto itr = this->find(window);
	this->checkItrEnd(itr);

	std::vector<Layout::AdjacentInfo>& parentAdjacent = Layout::GAdjacent.at(window.identifier);

	Layout::DockSpaceWindow newWindow{};
	newWindow.pos = { posX, itr->get()->pos.y };
	newWindow.size = itr->get()->size;

	Layout::AdjacentInfo adjacent{};
	adjacent.type = Layout::AdjacentInfo::Type::Left;
	adjacent.most = this->isRightMost(window);//if parent == rightmost -> splitted window(new window) == rightmost
	adjacent.window = window.identifier;

	std::shared_ptr<Layout::DockSpaceWindow> ptr = std::make_shared<Layout::DockSpaceWindow>(newWindow);
	LayoutData::windows.emplace_back(std::move(ptr));
	LayoutData::windows.back()->identifier = LayoutData::windows.back().get();

	//new window
	Layout::GAdjacent.insert({ LayoutData::windows.back().get(), { adjacent } });

	//update
	{
		Layout::AdjacentInfo info{};
		info.type = Layout::AdjacentInfo::Type::Right;
		info.most = adjacent.most;
		info.window = LayoutData::windows.back().get();
		parentAdjacent.emplace_back(info);
	}
}

void FD::LayoutWrite::splitHorizonal(const Layout::DockSpaceWindow& window, const float posY) {

}

void FD::LayoutWrite::splitCross(const Layout::DockSpaceWindow& window, const ImVec2& pos) {

}

void FD::LayoutWrite::update(const Layout::DockSpaceWindow& window) {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	auto itr = this->find(window);

	if (itr == LayoutData::windows.end())
		throw std::runtime_error("itr == end.");

	Layout::DockSpaceWindow* old = itr->get();
	if (
		static_cast<uint32_t>(old->pos.x) == static_cast<uint32_t>(window.pos.x) &&
		static_cast<uint32_t>(old->pos.y) == static_cast<uint32_t>(window.pos.y) &&
		static_cast<uint32_t>(old->size.x) == static_cast<uint32_t>(window.size.x) &&
		static_cast<uint32_t>(old->size.y) == static_cast<uint32_t>(window.size.y)
		)
	{
		return;
	}
	*old = window;

	std::vector<Layout::AdjacentInfo>& adjacent = Layout::GAdjacent.at(window.identifier);
	for (auto& x : adjacent) {
		using enum Layout::AdjacentInfo::Type;

		if (x.type == Left) {
			x.window->size.x = window.pos.x - x.window->pos.x;
		}
		else if (x.type == Right) {
			x.window->pos.x = window.pos.x + window.size.x;

			if (x.most) //rightmost
				x.window->size.x = LayoutData::rightPos.x - x.window->pos.x;
			else
				x.window->size.x = Layout::getRightPos(window.identifier).x;
		}
		else if (x.type == Top) {

		}
		else {
			assert(x.type == Bottom);
		}

	}


}

void FD::LayoutWrite::save() const noexcept {
	using namespace Layout::Internal;
	LayoutData::save.store(true);
}

std::vector<std::shared_ptr<FD::Layout::DockSpaceWindow>>::iterator FD::LayoutWrite::find(const Layout::DockSpaceWindow& window) const {
	using namespace Layout::Internal;
	return std::find_if(LayoutData::windows.begin(), LayoutData::windows.end(), [&](auto& x)
		{
			return window.identifier == x.get();
		}
	);
}

void FD::LayoutWrite::checkItrEnd(const std::vector<std::shared_ptr<Layout::DockSpaceWindow>>::iterator itr) const {
	if (itr == Layout::Internal::LayoutData::windows.end())
		throw std::runtime_error("itr == end().");
}

bool FD::LayoutWrite::isRightMost(const Layout::DockSpaceWindow& window) const {
	auto& vec = Layout::GAdjacent.at(window.identifier);

	for (auto& x : vec) {
		if (x.type == Layout::AdjacentInfo::Type::Right)
			return false;
	}

	return true;
}

bool FD::LayoutWrite::isBottomMost(const Layout::DockSpaceWindow& window) const {
	auto& vec = Layout::GAdjacent.at(window.identifier);

	for (auto& x : vec) {
		if (x.type == Layout::AdjacentInfo::Type::Bottom)
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const ImVec2& FD::LayoutRead::leftPos() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::leftPos;
}

const ImVec2& FD::LayoutRead::rightPos() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::rightPos;
}

FD::Layout::DockSpaceWindow FD::LayoutRead::get(const uint16_t index) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return *LayoutData::windows.at(index).get();
}

bool FD::LayoutRead::empty() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::windows.empty();
}

uint16_t FD::LayoutRead::size() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return static_cast<uint16_t>(LayoutData::windows.size());
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