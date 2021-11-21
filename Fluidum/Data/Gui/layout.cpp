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
	struct {
		float widthLimit{};
		float heightLimit{};
	}GData;

	struct AdjacentInfo final {
		bool horizonal{};
		std::vector<std::shared_ptr<float>> separators{};

		float* left = nullptr;
		float* right = nullptr;
		float* top = nullptr;
		float* bottom = nullptr;

		bool haveChild = false;
	};
	std::vector<std::shared_ptr<AdjacentInfo>> GAdjacent{};

	uint16_t separatorSize() {
		uint16_t count = 0;
		for (auto& x : GAdjacent) {
			count += static_cast<uint16_t>(x->separators.size());
		}
		return count;
	}

	//left right top bottom
	std::array<float*, 4> getSeparators(AdjacentInfo* info, const float val) {
		if (info->separators.empty()) {
			return { info->left,info->right,info->top,info->bottom };
		}

		const uint16_t size = static_cast<uint16_t>(info->separators.size());

		//left
		if (val < *info->separators[0]) {
			assert(val > *info->left);
			return { info->left,info->separators[0].get(),info->top,info->bottom };
		}
		//right
		if (val > *info->separators[size - 1]) {
			assert(val < *info->right);
			return { info->separators[size - 1].get(),info->right,info->top,info->bottom };
		}

		assert(size >= 2);
		for (uint16_t i = 1; i < size; i++) {
			if (val < *info->separators[i]) {
				return { info->separators[i - 1].get(),info->separators[i].get(),info->top,info->bottom };
			}
		}

		throw std::runtime_error("Failed to find separators.");
	}

	Internal::ResizedBorder border(const DockSpaceWindow& resizedWindow, const DockSpaceWindow* oldWindow) noexcept {
		using enum Internal::ResizedBorder;

		if (static_cast<int32_t>(resizedWindow.pos.x) != static_cast<int32_t>(oldWindow->pos.x))
			return Left;
		else if (static_cast<int32_t>(resizedWindow.size.x) != static_cast<int32_t>(oldWindow->size.x)) //old pos.x == new pos.x
			return Right;
		else if (static_cast<int32_t>(resizedWindow.pos.y) != static_cast<int32_t>(oldWindow->pos.y))
			return Top;
		else if (static_cast<int32_t>(resizedWindow.size.y) != static_cast<int32_t>(oldWindow->size.y))
			return Bottom;
		else
			return None;
	}
}

void FD::LayoutWrite::mainFrameLeft(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	LayoutData::mainFrameLeft = val;
}

void FD::LayoutWrite::mainFrameRight(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	LayoutData::mainFrameRight = val;
}

void FD::LayoutWrite::mainFrameTop(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	LayoutData::mainFrameTop = val;
}

void FD::LayoutWrite::mainFrameBottom(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	LayoutData::mainFrameBottom = val;
}

void FD::LayoutWrite::widthLimit(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	Layout::GData.widthLimit = val;
}

void FD::LayoutWrite::heightLimit(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	Layout::GData.heightLimit = val;
}

void FD::LayoutWrite::reset() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	LayoutData::windows.clear();
	Layout::GAdjacent.clear();

	Layout::AdjacentInfo info{};
	info.left = &LayoutData::mainFrameLeft;
	info.right = &LayoutData::mainFrameRight;
	info.top = &LayoutData::mainFrameTop;
	info.bottom = &LayoutData::mainFrameBottom;
	info.horizonal = false;
	Layout::GAdjacent.emplace_back(std::make_shared<Layout::AdjacentInfo>(info));

	Layout::DockSpaceWindow window{};
	window.identifier = Layout::GAdjacent.back().get();
	window.pos = { LayoutData::mainFrameLeft,LayoutData::mainFrameTop };
	window.size = { LayoutData::mainFrameRight - window.pos.x,LayoutData::mainFrameBottom - window.pos.y };
	window.minSize = { 0.0f,0.0f };
	window.maxSize = { std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };

	LayoutData::windows.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
	LayoutData::windows.back().get()->window = LayoutData::windows.back().get();
}

bool FD::LayoutWrite::splitVertical(const Layout::DockSpaceWindow& window, const float posX) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);
	assert(!layout->haveChild);

	if (Layout::separatorSize() >= Layout::Limits::Window::SeparatorMax)
		return false;

	if (!layout->horizonal) {
		layout->separators.emplace_back(std::make_shared<float>(posX));
		std::sort(layout->separators.begin(), layout->separators.end(), [](auto& x, auto& y) { return *x.get() < *y.get(); });
	}
	else {
		auto separators = Layout::getSeparators(layout, posX);
		Layout::AdjacentInfo info{};
		info.left = separators[0];
		info.right = separators[1];
		info.top = separators[2];
		info.bottom = separators[3];
		info.horizonal = false;
		Layout::GAdjacent.emplace_back(std::make_shared<Layout::AdjacentInfo>(info));
	}

	this->remakeAllWindows();

	return true;
}

bool FD::LayoutWrite::splitHorizonal(const Layout::DockSpaceWindow& window, const float posY) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);
	assert(!layout->haveChild);

	if (Layout::separatorSize() >= Layout::Limits::Window::SeparatorMax)
		return false;

	if (layout->horizonal) {
		layout->separators.emplace_back(std::make_shared<float>(posY));
		std::sort(layout->separators.begin(), layout->separators.end(), [](auto& x, auto& y) { return *x.get() < *y.get(); });
	}
	else {
		auto separators = Layout::getSeparators(layout, posY);
		Layout::AdjacentInfo info{};
		info.left = separators[0];
		info.right = separators[1];
		info.top = separators[2];
		info.bottom = separators[3];
		info.horizonal = false;
		Layout::GAdjacent.emplace_back(std::make_shared<Layout::AdjacentInfo>(info));
	}

	this->remakeAllWindows();

	return true;
}

bool FD::LayoutWrite::splitCross(const Layout::DockSpaceWindow& window, const ImVec2& pos) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	return true;
}

void FD::LayoutWrite::update(const Layout::DockSpaceWindow& window) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	const Layout::Internal::ResizedBorder border = Layout::border(window, window.window);
	using enum Layout::Internal::ResizedBorder;
	if (border == None)
		return;

	Layout::AdjacentInfo* info = static_cast<Layout::AdjacentInfo*>(window.identifier);

	//left right top bottom
	std::array<float*, 4> separators{};

	if (info->horizonal)
		abort();// separators = Layout::getSeparators(info, window.window->pos.y + (window.window->size.y / 2.0f));
	else
		separators = Layout::getSeparators(info, window.window->pos.x + (window.window->size.x / 2.0f));

	if (border == Left) {
		*separators[0] = window.pos.x;
	}
	else if (border == Right) {
		*separators[1] = window.pos.x + window.size.x;
	}
	else if (border == Top) {
		*separators[2] = window.pos.y;
	}
	else { //bottom
		assert(border == Bottom);
		*separators[3] = window.pos.y + window.size.y;
	}
	this->remakeAllWindows(border);
}

void FD::LayoutWrite::save() const noexcept {
	using namespace Layout::Internal;
	LayoutData::save.store(true);
}

void FD::LayoutWrite::remakeAllWindows(const Layout::Internal::ResizedBorder border) const {
	//main layout window
	if (Layout::GAdjacent.size() == 1 && Layout::GAdjacent[0]->separators.empty()) {
		Layout::DockSpaceWindow window{};
		window.identifier = Layout::GAdjacent[0].get();
		window.pos = { *Layout::GAdjacent[0]->left,*Layout::GAdjacent[0]->top };
		window.size = { *Layout::GAdjacent[0]->right - window.pos.x ,*Layout::GAdjacent[0]->bottom - window.pos.y };
		window.minSize = { 0.0f,0.0f };
		window.maxSize = { std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };
		Layout::Internal::LayoutData::windows = { std::make_shared<Layout::DockSpaceWindow>(window) };
		Layout::Internal::LayoutData::windows.back().get()->window = Layout::Internal::LayoutData::windows.back().get();
		return;
	}

	std::vector<std::shared_ptr<Layout::DockSpaceWindow>> result{};
	result.reserve(Layout::Internal::LayoutData::windows.size() + 2);
	assert(!Layout::GAdjacent.empty());


	for (auto& x : Layout::GAdjacent) {

		if (x.get()->haveChild)
			continue;

		Layout::DockSpaceWindow window{};
		window.identifier = x.get();
		window.minSize = { Layout::GData.widthLimit  ,Layout::GData.heightLimit };

		if (x->separators.empty()) {
			window.pos = { *x->left,*x->top };
			window.size = { *x->right - window.pos.x ,*x->bottom - window.pos.y };
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
		else {
			//first
			window.pos = { *x->left,*x->top };
			window.size = { *x->separators[0] - window.pos.x,*x->bottom - window.pos.y };
			window.maxSize = { (x->separators.size() == 1 ? *x->right : *x->separators[1]) - window.pos.x - Layout::GData.widthLimit ,10000 };
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();

			for (uint16_t i = 1, size = static_cast<uint16_t>(x->separators.size()); i < size; i++) {
				window.pos = { *x->separators[i - 1],*x->top };
				window.size = { *x->separators[i] - window.pos.x,*x->bottom - window.pos.y };

				if (border == Layout::Internal::ResizedBorder::Left)
					window.maxSize = { window.pos.x + window.size.x - Layout::GData.widthLimit - (i == 1 ? *x->left : *x->separators[i - 2]),10000 };
				else if (border == Layout::Internal::ResizedBorder::Right)
					window.maxSize = { (i == x->separators.size() - 1 ? *x->right : *x->separators[i + 1]) - window.pos.x - Layout::GData.widthLimit,10000 };
				else
					window.maxSize = { std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };

				result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
				result.back().get()->window = result.back().get();
			}

			//last
			window.pos = { *x->separators[x->separators.size() - 1],*x->top };
			window.size = { *x->right - window.pos.x,*x->bottom - window.pos.y };
			window.maxSize = { window.pos.x + window.size.x - Layout::GData.widthLimit - (x->separators.size() == 1 ? *x->left : *x->separators[x->separators.size() - 2]),10000 };
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}

	}
	Layout::Internal::LayoutData::windows = std::move(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FD::LayoutRead::mainFrameLeft() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::mainFrameLeft;
}

float FD::LayoutRead::mainFrameRight() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::mainFrameRight;
}

float FD::LayoutRead::mainFrameTop() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::mainFrameTop;
}

float FD::LayoutRead::mainFrameBottom() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::mainFrameBottom;
}

float FD::LayoutRead::widthLimit() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return Layout::GData.widthLimit;
}

float FD::LayoutRead::heightLimit() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return Layout::GData.heightLimit;
}

std::vector<FD::Layout::DockSpaceWindow> FD::LayoutRead::get() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	std::vector<FD::Layout::DockSpaceWindow> ret{};
	ret.reserve(LayoutData::windows.size());

	for (auto& x : LayoutData::windows) {
		ret.emplace_back(*x.get());
	}
	return ret;
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