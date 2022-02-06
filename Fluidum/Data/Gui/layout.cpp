#include "layout.h"

namespace FD::Layout {
	struct {
		float widthLimit{};
		float heightLimit{};
	}GData;

	using Separator = float;

	struct SeparatorInfo final {
		std::shared_ptr<Separator> separator = nullptr;

		std::shared_ptr<Separator> left = nullptr;
		std::shared_ptr<Separator> right = nullptr;
		std::shared_ptr<Separator> top = nullptr;
		std::shared_ptr<Separator> bottom = nullptr;
	};

	struct AdjacentInfo final {
		bool horizonal{};
		std::vector<std::shared_ptr<SeparatorInfo>> separators{};

		std::shared_ptr<SeparatorInfo> left = nullptr;
		std::shared_ptr<SeparatorInfo> right = nullptr;
		std::shared_ptr<SeparatorInfo> top = nullptr;
		std::shared_ptr<SeparatorInfo> bottom = nullptr;

		//0, 1, 2, ...,
		//nullptr -> rightmost or bottommost
		std::vector<std::shared_ptr<SeparatorInfo>> haveChildren{};

		//for merge
		std::shared_ptr<AdjacentInfo> parent = nullptr;
		std::shared_ptr<SeparatorInfo> child = nullptr;
	};

	std::vector<std::shared_ptr<AdjacentInfo>> GAdjacent{};

	std::shared_ptr<FD::Layout::SeparatorInfo> GUpdateSeparator = nullptr;

	std::shared_ptr<AdjacentInfo> find(AdjacentInfo* elm) {
		auto itr = std::find_if(GAdjacent.begin(), GAdjacent.end(), [=](auto& x) {return x.get() == elm; });
		if (itr == GAdjacent.end())
			throw std::runtime_error("Failed to find GAdjacent.");
		return *itr;
	}

	UIF16 separatorSize() {
		UIF16 count = 0;
		for (auto& x : GAdjacent) {
			count += static_cast<UIF16>(x->separators.size());
		}
		return count;
	}

	//child : left right top bottom
	std::pair<std::shared_ptr<Layout::SeparatorInfo>, std::array<std::shared_ptr<Layout::SeparatorInfo>, 4>> getSeparators(AdjacentInfo* info, const float val) {
		if (info->separators.empty()) {
			return { nullptr, {info->left, info->right, info->top, info->bottom} };
		}

		const uint16_t size = static_cast<uint16_t>(info->separators.size());

		if (info->horizonal) {
			//top
			if (val < *info->separators[0]->separator) {
				assert(val > *info->top->separator);
				return { info->separators[0], {info->left, info->right, info->top, info->separators[0]} };
			}
			//bottom
			if (val > *info->separators[size - 1]->separator) {
				assert(val < *info->bottom->separator);
				return { nullptr,{ info->left, info->right, info->separators[size - 1], info->bottom } };
			}

			assert(size >= 2);
			for (uint16_t i = 1; i < size; i++) {
				if (val < *info->separators[i]->separator) {
					return { info->separators[i],{info->left,info->right, info->separators[i - 1],info->separators[i] } };
				}
			}
		}
		else {
			//left
			if (val < *info->separators[0]->separator) {
				assert(val > *info->left->separator);
				return { info->separators[0],{ info->left,info->separators[0],info->top,info->bottom } };
			}
			//right
			if (val > *info->separators[size - 1]->separator) {
				assert(val < *info->right->separator);
				return { nullptr,{ info->separators[size - 1],info->right,info->top,info->bottom } };
			}

			assert(size >= 2);
			for (uint16_t i = 1; i < size; i++) {
				if (val < *info->separators[i]->separator) {
					return { info->separators[i],{ info->separators[i - 1], info->separators[i], info->top, info->bottom } };
				}
			}
		}

		throw std::runtime_error("Failed to find separators.");
	}

	Internal::ResizedBorder border(const DockSpaceWindow* oldWindow, const ImVec2& pos) noexcept {
		using enum Internal::ResizedBorder;

		//left right top bottom
		std::array<float, 4> distance{
			std::fabs(pos.x - oldWindow->pos.x),
			std::fabs(pos.x - (oldWindow->pos.x + oldWindow->size.x)),
			std::fabs(pos.y - oldWindow->pos.y),
			std::fabs(pos.y - (oldWindow->pos.y + oldWindow->size.y))
		};

		auto itr = std::min_element(distance.begin(), distance.end());

		if (*itr > 17.0f)
			return None;

		const auto dis = std::distance(distance.begin(), itr);

		if (dis == 0)
			return Left;
		else if (dis == 1)
			return Right;
		else if (dis == 2)
			return Top;
		else if (dis == 3)
			return Bottom;
		return None;
	}

	[[nodiscard]] bool isMainFrameOnly() {
		return (GAdjacent.size() == 1 && GAdjacent[0]->separators.empty());
	}

	void round(DockSpaceWindow& window) {
		window.pos.x = std::roundf(window.pos.x);
		window.pos.y = std::roundf(window.pos.y);
		window.size.x = std::roundf(window.size.x);
		window.size.y = std::roundf(window.size.y);
	}

	void round_x(DockSpaceWindow& window) {
		window.pos.x = std::roundf(window.pos.x);
		window.size.x = std::roundf(window.size.x);
	}

	void round_y(DockSpaceWindow& window) {
		window.pos.y = std::roundf(window.pos.y);
		window.size.y = std::roundf(window.size.y);
	}

	[[nodiscard]] uint16_t findSeparatorIndex(AdjacentInfo* info, std::shared_ptr<SeparatorInfo>& separator) {
		auto itr = std::find(info->separators.begin(), info->separators.end(), separator);
		if (itr == info->separators.end())
			throw std::runtime_error("Failed to find separator.");
		return static_cast<uint16_t>(std::distance(info->separators.begin(), itr));
	}

	void updateSeparators_Vertical() {
		for (auto& x : GAdjacent) {
			if (x->horizonal)
				continue;

			for (auto& y : GAdjacent) {
				if (y->horizonal)
					continue;

				if (x == y) {
					for (auto& sep : y->separators) {
						for (auto& z : x->separators) {
							if (sep == z)
								continue;

							//new left
							if (*sep->separator > *z->left && *sep->separator < *z->separator)
								z->left = sep->separator;
							else if (*sep->separator < *z->right && *sep->separator > *z->separator) //new right
								z->right = sep->separator;
						}
					}
					continue;
				}

				//left right
				for (auto& sep : y->separators) {
					if (!(*x->top->separator <= *y->top->separator && *x->bottom->separator >= *y->bottom->separator))
						continue;

					//new left
					if (*sep->separator > *x->left->left && *sep->separator < *x->left->separator)
						x->left->left = sep->separator;
					else if (*sep->separator < *x->right->right && *sep->separator > *x->right->separator) //new right
						x->right->right = sep->separator;
				}

				//separators
				for (auto& sep : y->separators) {
					if (!(*x->top->separator <= *y->top->separator && *x->bottom->separator >= *y->bottom->separator))
						continue;

					for (auto& z : x->separators) {
						//new left
						if (*sep->separator > *z->left && *sep->separator < *z->separator)
							z->left = sep->separator;
						else if (*sep->separator < *z->right && *sep->separator > *z->separator) //new right
							z->right = sep->separator;
					}
				}

			}
		}
	}

	void updateSeparators_Horizonal() {
		for (auto& x : GAdjacent) {
			if (!x->horizonal)
				continue;

			for (auto& y : GAdjacent) {
				if (!y->horizonal)
					continue;

				if (x == y) {
					for (auto& sep : y->separators) {
						for (auto& z : x->separators) {
							if (sep == z)
								continue;

							//new top
							if (*sep->separator > *z->top && *sep->separator < *z->separator)
								z->top = sep->separator;
							else if (*sep->separator < *z->bottom && *sep->separator > *z->separator) //new bottom
								z->bottom = sep->separator;
						}
					}
					continue;
				}


				//top bottom
				for (auto& sep : y->separators) {
					if (!(*x->left->separator <= *y->left->separator && *x->right->separator >= *y->right->separator))
						continue;

					//new top
					if (*sep->separator > *x->top->top && *sep->separator < *x->top->separator)
						x->top->top = sep->separator;
					else if (*sep->separator < *x->bottom->bottom && *sep->separator > *x->bottom->separator) //new bottom
						x->bottom->bottom = sep->separator;
				}

				//separators
				for (auto& sep : y->separators) {
					if (!(*x->left->separator <= *y->left->separator && *x->right->separator >= *y->right->separator))
						continue;

					for (auto& z : x->separators) {
						//new top
						if (*sep->separator > *z->top && *sep->separator < *z->separator)
							z->top = sep->separator;
						else if (*sep->separator < *z->bottom && *sep->separator > *z->separator) //new bottom
							z->bottom = sep->separator;
					}
				}
			}
		}
	}

	[[nodiscard]] bool haveChildren(const std::shared_ptr<AdjacentInfo>& adj, const std::shared_ptr<SeparatorInfo>& sep) {
		auto find = std::find(adj->haveChildren.begin(), adj->haveChildren.end(), sep);
		return find != adj->haveChildren.end();
	}
}

void FD::Layout::Internal::LayoutData::remake() {
	using namespace Layout::Internal;

	std::vector<History> copy{};
	{
		std::lock_guard<std::mutex> lock(LayoutData::mtx);
		copy = history;
	}

	LayoutWrite::reset();

	const float layoutWindowWidth = *LayoutData::mainFrameRight - *LayoutData::mainFrameLeft;
	const float layoutWindowHeight = *LayoutData::mainFrameBottom - *LayoutData::mainFrameTop;

	try {
		for (auto& x : copy) {
			const float readPosX = *LayoutData::mainFrameLeft + (layoutWindowWidth * x.readPosRatio.x);
			const float readPosY = *LayoutData::mainFrameTop + (layoutWindowHeight * x.readPosRatio.y);

			DockSpaceWindow* win = findWindow({ readPosX, readPosY });
			if (!win)
				throw std::runtime_error("findWindow() returned nullptr.");

			if (x.horizonal)
				LayoutWrite::splitHorizonal(*win, readPosY);
			else
				LayoutWrite::splitVertical(*win, readPosX);

			LayoutWrite::remakeAllWindows();
		}
	}
	catch (const std::exception&) {
		GAdjacent.clear();
		std::rethrow_exception(std::current_exception());
	}
}

FD::Layout::DockSpaceWindow* FD::Layout::Internal::LayoutData::findWindow(const ImVec2& pos) {
	for (auto& x : windows) {
		if (
			x->pos.x < pos.x &&
			x->pos.y < pos.y &&
			x->pos.x + x->size.x > pos.x &&
			x->pos.y + x->size.y > pos.y
			)
		{
			return x.get();
		}
	}

	return nullptr;
}

void FD::LayoutWrite::mainFrameLeft(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	if (LayoutData::mainFrameLeft)
		*LayoutData::mainFrameLeft.get() = val;
	else
		LayoutData::mainFrameLeft = std::make_shared<Layout::Separator>(val);
}

void FD::LayoutWrite::mainFrameRight(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	if (LayoutData::mainFrameRight)
		*LayoutData::mainFrameRight.get() = val;
	else
		LayoutData::mainFrameRight = std::make_shared<Layout::Separator>(val);
}

void FD::LayoutWrite::mainFrameTop(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	if (LayoutData::mainFrameTop)
		*LayoutData::mainFrameTop.get() = val;
	else
		LayoutData::mainFrameTop = std::make_shared<Layout::Separator>(val);
}

void FD::LayoutWrite::mainFrameBottom(const float val) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	if (LayoutData::mainFrameBottom)
		*LayoutData::mainFrameBottom.get() = val;
	else
		LayoutData::mainFrameBottom = std::make_shared<Layout::Separator>(val);
}

void FD::LayoutWrite::resizeMainFrameRight(const float val) {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	*LayoutData::mainFrameRight = val;

	for (auto& x : Layout::GAdjacent) {
		for (auto& y : x->separators) {
			if ((*y->right - *y->separator) <= Layout::GData.widthLimit) {
				*y->separator = (*y->right - Layout::GData.widthLimit);
			}
		}
	}

	this->remakeAllWindows();
}

void FD::LayoutWrite::resizeMainFrameBottom(const float val) {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	*LayoutData::mainFrameBottom = val;

	for (auto& x : Layout::GAdjacent) {
		for (auto& y : x->separators) {
			if ((*y->bottom - *y->separator) <= Layout::GData.heightLimit) {
				*y->separator = (*y->bottom - Layout::GData.heightLimit);
			}
		}
	}

	this->remakeAllWindows();
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

float FD::Layout::UnRedoInfo::calcMidpoint(const float first, const float last) const {
	assert(first < last);
	return first + ((last - first) / 2.0f);
}

void FD::LayoutWrite::reset() {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	LayoutData::windows.clear();
	LayoutData::history.clear();
	LayoutData::unredo.clear();
	Layout::GAdjacent.clear();

	Layout::SeparatorInfo sInfo{};
	sInfo.left = LayoutData::mainFrameLeft;
	sInfo.right = LayoutData::mainFrameRight;
	sInfo.top = LayoutData::mainFrameTop;
	sInfo.bottom = LayoutData::mainFrameBottom;

	Layout::AdjacentInfo info{};
	sInfo.separator = LayoutData::mainFrameLeft;
	info.left = std::make_shared<Layout::SeparatorInfo>(sInfo);
	sInfo.separator = LayoutData::mainFrameRight;
	info.right = std::make_shared<Layout::SeparatorInfo>(sInfo);
	sInfo.separator = LayoutData::mainFrameTop;
	info.top = std::make_shared<Layout::SeparatorInfo>(sInfo);
	sInfo.separator = LayoutData::mainFrameBottom;
	info.bottom = std::make_shared<Layout::SeparatorInfo>(sInfo);
	info.horizonal = false;
	Layout::GAdjacent.emplace_back(std::make_shared<Layout::AdjacentInfo>(info));


	remakeAllWindows();
}

void FD::LayoutWrite::remake() {
	Layout::Internal::LayoutData::remake();
}

bool FD::LayoutWrite::splitVertical(const Layout::DockSpaceWindow& window, const float posX) {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	if (Layout::separatorSize() >= Layout::Limits::Window::SeparatorMax)
		return false;

	if (Layout::isMainFrameOnly())
		layout->horizonal = false;

	std::shared_ptr<Layout::Separator> newSeparator{};
	std::shared_ptr<Layout::Separator> top_bottom[2]{};

	if (!layout->horizonal) {
		auto separators = Layout::getSeparators(layout, posX).second;
		Layout::SeparatorInfo info{};
		info.separator = std::make_shared<Layout::Separator>(posX);
		info.left = separators[0]->separator;
		info.right = separators[1]->separator;
		info.top = separators[2]->top;
		info.bottom = separators[3]->bottom;
		layout->separators.emplace_back(std::make_shared<Layout::SeparatorInfo>(std::move(info)));
		newSeparator = layout->separators.back()->separator;
		top_bottom[0] = layout->top->separator;
		top_bottom[1] = layout->bottom->separator;
		std::sort(layout->separators.begin(), layout->separators.end(), [](auto& x, auto& y) { return *x.get()->separator < *y.get()->separator; });
	}
	else {
		auto [child, separators] = Layout::getSeparators(layout, window.pos.y + (window.size.y / 2.0f));
		Layout::AdjacentInfo info{};
		info.left = separators[0];
		info.right = separators[1];
		info.top = separators[2];
		info.bottom = separators[3];
		info.horizonal = false;
		info.parent = Layout::find(layout);
		info.child = child;

		Layout::SeparatorInfo sInfo{};
		sInfo.separator = std::make_shared<Layout::Separator>(posX);
		sInfo.left = separators[0]->separator;
		sInfo.right = separators[1]->separator;
		sInfo.top = separators[2]->top;
		sInfo.bottom = separators[3]->bottom;

		Layout::GAdjacent.emplace_back(std::make_shared<Layout::AdjacentInfo>(info));
		Layout::GAdjacent.back()->separators.emplace_back(std::make_shared<Layout::SeparatorInfo>(std::move(sInfo)));
		newSeparator = Layout::GAdjacent.back()->separators.back()->separator;
		top_bottom[0] = separators[2]->separator;
		top_bottom[1] = separators[3]->separator;
		layout->haveChildren.emplace_back(child);
	}

	Layout::updateSeparators_Vertical();

	//history
	::FD::Layout::Internal::History his{};
	auto itr = std::find_if(LayoutData::windows.begin(), LayoutData::windows.end(), [&](auto& x) {return x.get() == window.window; });
	assert(itr != LayoutData::windows.end());
	his.horizonal = false;
	his.pos_side1 = top_bottom[0].get();
	his.pos_side2 = top_bottom[1].get();
	his.pos = newSeparator.get();
	LayoutData::history.emplace_back(his);

	//Layout::UnRedoInfo urInfo{};
	//urInfo.separator = newSeparator.get();
	//urInfo.type = Layout::UnRedoInfo::Type::Split;
	//urInfo.pos1 = posX;
	//urInfo.pos2 = urInfo.calcMidpoint(*layout->top->separator, *layout->bottom->separator);
	//if (LayoutData::unredo.size() >= Layout::Limits::UnRedo)//pop front elm -> push back
	//	LayoutData::unredo.erase(LayoutData::unredo.begin());
	//LayoutData::unredo.emplace_back(urInfo);

	remakeAllWindows();

	return true;
}

bool FD::LayoutWrite::splitHorizonal(const Layout::DockSpaceWindow& window, const float posY) {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	if (Layout::isMainFrameOnly())
		layout->horizonal = true;

	if (Layout::separatorSize() >= Layout::Limits::Window::SeparatorMax)
		return false;

	std::shared_ptr<Layout::Separator> newSeparator{};
	std::shared_ptr<Layout::Separator> left_right[2]{};


	if (layout->horizonal) {
		auto separators = Layout::getSeparators(layout, posY).second;
		Layout::SeparatorInfo info{};
		info.separator = std::make_shared<Layout::Separator>(posY);
		info.left = separators[0]->left;
		info.right = separators[1]->right;
		info.top = separators[2]->separator;
		info.bottom = separators[3]->separator;
		layout->separators.emplace_back(std::make_shared<Layout::SeparatorInfo>(std::move(info)));
		newSeparator = layout->separators.back()->separator;
		left_right[0] = layout->left->separator;
		left_right[1] = layout->right->separator;
		std::sort(layout->separators.begin(), layout->separators.end(), [](auto& x, auto& y) { return *x.get()->separator < *y.get()->separator; });
	}
	else {
		auto [child, separators] = Layout::getSeparators(layout, window.pos.x + (window.size.x / 2.0f));
		Layout::AdjacentInfo info{};
		info.left = separators[0];
		info.right = separators[1];
		info.top = separators[2];
		info.bottom = separators[3];
		info.horizonal = true;
		info.parent = Layout::find(layout);
		info.child = child;

		Layout::SeparatorInfo sInfo{};
		sInfo.separator = std::make_shared<Layout::Separator>(posY);
		sInfo.left = separators[0]->left;
		sInfo.right = separators[1]->right;
		sInfo.top = separators[2]->separator;
		sInfo.bottom = separators[3]->separator;

		Layout::GAdjacent.emplace_back(std::make_shared<Layout::AdjacentInfo>(info));
		Layout::GAdjacent.back()->separators.emplace_back(std::make_shared<Layout::SeparatorInfo>(std::move(sInfo)));
		newSeparator = Layout::GAdjacent.back()->separators.back()->separator;
		left_right[0] = separators[0]->separator;
		left_right[1] = separators[1]->separator;
		layout->haveChildren.emplace_back(child);
	}

	Layout::updateSeparators_Horizonal();

	//history
	::FD::Layout::Internal::History his{};
	his.horizonal = true;
	auto itr = std::find_if(LayoutData::windows.begin(), LayoutData::windows.end(), [&](auto& x) {return x.get() == window.window; });
	his.pos_side1 = left_right[0].get();
	his.pos_side2 = left_right[1].get();
	his.pos = newSeparator.get();
	LayoutData::history.emplace_back(his);

	//Layout::UnRedoInfo urInfo{};
	//urInfo.separator = newSeparator.get();
	//urInfo.type = Layout::UnRedoInfo::Type::Split;
	//urInfo.pos1 = posY;
	//urInfo.pos2 = urInfo.calcMidpoint(*layout->left->separator, *layout->right->separator);
	//if (LayoutData::unredo.size() >= Layout::Limits::UnRedo)//pop front elm -> push back
	//	LayoutData::unredo.erase(LayoutData::unredo.begin());
	//LayoutData::unredo.emplace_back(urInfo);

	remakeAllWindows();

	return true;
}

bool FD::LayoutWrite::merge(const Layout::DockSpaceWindow& window, const ImVec2& pos) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	std::array<std::shared_ptr<Layout::SeparatorInfo>, 4> separators{};

	if (layout->horizonal)
		separators = Layout::getSeparators(layout, pos.y).second;
	else
		separators = Layout::getSeparators(layout, pos.x).second;

	//left right top bottom
	std::array<float, 4> distance = {
		pos.x - *separators[0]->separator,
		*separators[1]->separator - pos.x,
		pos.y - *separators[2]->separator,
		*separators[3]->separator - pos.y
	};
	const auto itr = std::min_element(distance.begin(), distance.end());
	const auto min = std::distance(distance.begin(), itr);
	assert(min < 4);

	//left right
	if (min == 0 || min == 1) {
		for (auto& x : Layout::GAdjacent) {
			if (x->left == separators[min] ||
				x->right == separators[min]) {
				return false;
			}
		}
	}
	else {//top bottom
		for (auto& x : Layout::GAdjacent) {
			if (x->top == separators[min] ||
				x->bottom == separators[min]) {
				return false;
			}
		}
	}

	//left or right
	if (min == 0 || min == 1) {
		auto sep = separators[min]->separator;

		for (auto& x : Layout::GAdjacent) {
			if (x->left->left == sep)
				x->left->left = Layout::GAdjacent[0]->left->left;
			else if (x->left->right == sep)
				x->left->right = Layout::GAdjacent[0]->right->right;

			else if (x->right->left == sep)
				x->right->left = Layout::GAdjacent[0]->left->left;
			else if (x->right->right == sep)
				x->right->right = Layout::GAdjacent[0]->right->right;

			else if (x->top->left == separators[min]->separator)
				x->top->left = Layout::GAdjacent[0]->left->left;
			else if (x->top->right == separators[min]->separator)
				x->top->right = Layout::GAdjacent[0]->right->right;

			else if (x->bottom->left == separators[min]->separator)
				x->bottom->left = Layout::GAdjacent[0]->left->left;
			else if (x->bottom->right == separators[min]->separator)
				x->bottom->right = Layout::GAdjacent[0]->right->right;

			if (x->left == separators[min])
				x->left = Layout::GAdjacent[0]->left;
			else if (x->right == separators[min])
				x->right = Layout::GAdjacent[0]->right;
		}

		for (auto& x : Layout::GAdjacent) {
			bool done = false;
			for (auto itr = x->separators.begin(), end = x->separators.end(); itr != end; itr++) {
				if (*itr == separators[min]) {
					x->separators.erase(itr);
					done = true;
					break;
				}
			}
			if (done) {
				auto itr = std::find(Layout::GAdjacent.begin() + 1, Layout::GAdjacent.end(), x);
				if (itr != Layout::GAdjacent.end()) {
					auto find = std::find(x->parent->haveChildren.begin(), x->parent->haveChildren.end(), x->child);
					if (find == x->parent->haveChildren.end())
						throw std::runtime_error("Failed to find children.");

					x->parent->haveChildren.erase(find);
					Layout::GAdjacent.erase(itr);
				}
				break;
			}
		}

		Layout::updateSeparators_Vertical();
	}
	else if (min == 2 || min == 3) {
		auto sep = separators[min]->separator;

		for (auto& x : Layout::GAdjacent) {
			if (x->left->top == sep)
				x->left->top = Layout::GAdjacent[0]->top->top;
			else if (x->left->bottom == sep)
				x->left->bottom = Layout::GAdjacent[0]->bottom->bottom;

			else if (x->right->top == sep)
				x->right->top = Layout::GAdjacent[0]->top->top;
			else if (x->right->bottom == sep)
				x->right->bottom = Layout::GAdjacent[0]->bottom->bottom;

			else if (x->top->top == separators[min]->separator)
				x->top->top = Layout::GAdjacent[0]->top->top;
			else if (x->top->bottom == separators[min]->separator)
				x->top->bottom = Layout::GAdjacent[0]->bottom->bottom;

			else if (x->bottom->top == separators[min]->separator)
				x->bottom->top = Layout::GAdjacent[0]->top->top;
			else if (x->bottom->bottom == separators[min]->separator)
				x->bottom->bottom = Layout::GAdjacent[0]->bottom->bottom;

			if (x->top == separators[min])
				x->top = Layout::GAdjacent[0]->top;
			else if (x->bottom == separators[min])
				x->bottom = Layout::GAdjacent[0]->bottom;
		}

		for (auto& x : Layout::GAdjacent) {
			bool done = false;
			for (auto itr = x->separators.begin(), end = x->separators.end(); itr != end; itr++) {
				if (*itr == separators[min]) {
					x->separators.erase(itr);
					done = true;
					break;
				}
			}
			if (done) {
				if (x->separators.empty()) {
					auto itr = std::find(Layout::GAdjacent.begin() + 1, Layout::GAdjacent.end(), x);
					if (itr != Layout::GAdjacent.end()) {
						auto find = std::find(x->parent->haveChildren.begin(), x->parent->haveChildren.end(), x->child);
						if (find == x->parent->haveChildren.end())
							throw std::runtime_error("Failed to find children.");

						x->parent->haveChildren.erase(find);
						Layout::GAdjacent.erase(itr);
					}
				}
				break;
			}
		}

		Layout::updateSeparators_Horizonal();
	}

	Layout::GUpdateSeparator = nullptr;

	assert(separators[min].use_count() == 1);

	//history
	auto his = &LayoutData::history;
	auto find = std::find_if(LayoutData::history.begin(), LayoutData::history.end(), [&](auto& x) {return x.pos == separators[min]->separator.get(); });
	if (find == LayoutData::history.end())
		throw std::runtime_error("Failed to find layout history.");
	LayoutData::history.erase(find);

	//Layout::UnRedoInfo urInfo{};
	//urInfo.type = Layout::UnRedoInfo::Type::Merge;
	//urInfo.pos1 = pos.x;
	//urInfo.pos2 = pos.y;
	//if (LayoutData::unredo.size() >= Layout::Limits::UnRedo)//pop front elm -> push back
	//	LayoutData::unredo.erase(LayoutData::unredo.begin());
	//LayoutData::unredo.emplace_back(urInfo);

	remakeAllWindows();
	return true;
}

FD::Layout::ResizedBorder FD::LayoutWrite::update(const Layout::DockSpaceWindow& window, const ImVec2& mousePos, Layout::ResizedBorder border) const {
	using namespace Layout::Internal;
	using enum Layout::Internal::ResizedBorder;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	if (border == None)
		border = Layout::border(window.window, mousePos);
	if (border == None)
		return None;

	Layout::AdjacentInfo* info = static_cast<Layout::AdjacentInfo*>(window.identifier);

	const float pos = info->horizonal ? window.window->pos.y + (window.window->size.y / 2.0f) : window.window->pos.x + (window.window->size.x / 2.0f);

	//left right top bottom
	auto separators = Layout::getSeparators(info, pos).second;

	if (border == Left) {
		*separators[0]->separator = window.pos.x;
		Layout::GUpdateSeparator = separators[0];
	}
	else if (border == Right) {
		*separators[1]->separator = window.pos.x + window.size.x;
		Layout::GUpdateSeparator = separators[1];
	}
	else if (border == Top) {
		*separators[2]->separator = window.pos.y;
		Layout::GUpdateSeparator = separators[2];
	}
	else { //bottom
		assert(border == Bottom);
		*separators[3]->separator = window.pos.y + window.size.y;
		Layout::GUpdateSeparator = separators[3];
	}
	this->remakeAllWindows(border);

	return border;
}

void FD::LayoutWrite::update(const Layout::SeparatorPos& separator, const ImVec2& delta) {
	using namespace Layout::Internal;
	using enum Layout::Internal::ResizedBorder;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::SeparatorInfo* info = static_cast<Layout::SeparatorInfo*>(separator.identifier);

	const IF32 del = separator.horizonal ? static_cast<IF32>(delta.y) : static_cast<IF32>(delta.x);

	//limit
	if (separator.horizonal) {
		if (del < 0 && (*info->separator - *info->top) <= (Layout::GData.heightLimit * 1.02f))
			return;
		if (del > 0 && (*info->bottom - *info->separator) <= (Layout::GData.heightLimit * 1.02f))
			return;
	}
	else {
		if (del < 0 && (*info->separator - *info->left) <= (Layout::GData.widthLimit * 1.02f))
			return;
		if (del > 0 && (*info->right - *info->separator) <= (Layout::GData.widthLimit * 1.02f))
			return;
	}

	*info->separator += static_cast<float>(del);

	this->remakeAllWindows();
}

void FD::LayoutWrite::resize(const Layout::DockSpaceWindow& window, const Layout::ResizedBorder border) const {
	if (border == Layout::ResizedBorder::None)
		return;

	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	using enum Layout::ResizedBorder;
	Layout::Separator* bor{};
	if (border == Left) {
		bor = layout->left->separator.get();
		Layout::updateSeparators_Vertical();
	}
	else if (border == Right) {
		bor = layout->right->separator.get();
		Layout::updateSeparators_Vertical();
	}
	else if (border == Top) {
		bor = layout->top->separator.get();
		Layout::updateSeparators_Horizonal();
	}
	else {
		bor = layout->bottom->separator.get();
		Layout::updateSeparators_Horizonal();
	}

	//Layout::UnRedoInfo urInfo{};
	//urInfo.type = Layout::UnRedoInfo::Type::Resize;
	//auto itr = std::find_if(LayoutData::unredo.rbegin(), LayoutData::unredo.rend(), [&](auto& x)
	//	{
	//		if (x.type == Layout::UnRedoInfo::Type::Resize && x.separator == bor)
	//			return true;
	//		if (x.type == Layout::UnRedoInfo::Type::Split && x.separator == bor)
	//			return true;
	//
	//		return false;
	//	}
	//);
	//urInfo.separator = bor;
	//urInfo.pos1 = itr->pos1;
	//urInfo.pos2 = *itr->separator;
	//if (LayoutData::unredo.size() >= Layout::Limits::UnRedo)//pop front elm -> push back
	//	LayoutData::unredo.erase(LayoutData::unredo.begin());
	//LayoutData::unredo.emplace_back(urInfo);
}

void FD::LayoutWrite::undo(const std::size_t count) const {
	//using namespace Layout::Internal;
	//std::lock_guard<std::mutex> lock(LayoutData::mtx);

	//assert(LayoutData::currentUnRedoIndex + count < LayoutData::unredo.size());

	//const std::size_t limit = LayoutData::currentUnRedoIndex + count >= LayoutData::unredo.size() ? LayoutData::unredo.size() : LayoutData::currentUnRedoIndex + count;
	//for (std::size_t i = LayoutData::currentUnRedoIndex; i < limit; i++) {
	//	if (LayoutData::unredo[i].type == Layout::UnRedoInfo::Type::Resize) {

	//	}
	//	else if (LayoutData::unredo[i].type == Layout::UnRedoInfo::Type::Split) {

	//	}
	//	else {

	//	}
	//}

}

void FD::LayoutWrite::redo(const std::size_t count) const {
	//ŽÀ‘•—\’è
}

void FD::LayoutWrite::save() const noexcept {
	using namespace Layout::Internal;
	LayoutData::save.store(true);
}

void FD::LayoutWrite::remakeAllWindows(const Layout::Internal::ResizedBorder border) {
	//main docking window
	if (Layout::isMainFrameOnly()) {
		Layout::DockSpaceWindow window{};
		window.identifier = Layout::GAdjacent[0].get();
		window.pos = { *Layout::GAdjacent[0]->left->separator ,*Layout::GAdjacent[0]->top->separator };
		window.size = { *Layout::GAdjacent[0]->right->separator - window.pos.x ,*Layout::GAdjacent[0]->bottom->separator - window.pos.y };
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
		if (x->horizonal)
			remakeWindow_horizonal(result, x, border);
		else
			remakeWindow_vertical(result, x, border);
	}
	Layout::Internal::LayoutData::windows = std::move(result);
}

void FD::LayoutWrite::remakeWindow_horizonal(std::vector<std::shared_ptr<Layout::DockSpaceWindow>>& result, auto& x, const Layout::Internal::ResizedBorder border) {
	using enum Layout::Internal::ResizedBorder;
	constexpr float floatMax = std::numeric_limits<float>::max();

	const uint16_t size = static_cast<uint16_t>(x->separators.size());

	Layout::DockSpaceWindow window{};
	window.identifier = x.get();

	//limit size x
	float maxSizeX = floatMax;
	float minSizeX = Layout::GData.widthLimit;
	if (!(x->left->separator == Layout::Internal::LayoutData::mainFrameLeft && x->right->separator == Layout::Internal::LayoutData::mainFrameRight)) {
		if (border == Layout::Internal::ResizedBorder::Left) {
			if (x->left == Layout::GUpdateSeparator) {
				maxSizeX = *x->right->separator
					- *x->left->left
					- Layout::GData.widthLimit;
				minSizeX = -*x->left->right
					+ *x->right->separator
					+ Layout::GData.widthLimit;
			}
		}
		else if (border == Layout::Internal::ResizedBorder::Right) {
			if (x->right == Layout::GUpdateSeparator) {
				maxSizeX = *x->right->right
					- *x->left->separator
					- Layout::GData.widthLimit;
				minSizeX = *x->right->left
					- *x->left->separator
					+ Layout::GData.widthLimit;
			}
		}
		else if (border == Layout::Internal::ResizedBorder::Top) {
			if (x->top == Layout::GUpdateSeparator) {
				minSizeX = -*x->top->bottom
					+ *x->bottom->separator
					+ Layout::GData.widthLimit;
			}
		}
		else if (border == Layout::Internal::ResizedBorder::Bottom) {
			if (x->bottom == Layout::GUpdateSeparator) {
				minSizeX = *x->bottom->top
					- *x->top->separator
					+ Layout::GData.widthLimit;
			}
		}

		if (minSizeX < Layout::GData.widthLimit)
			minSizeX = Layout::GData.widthLimit;
	}

	window.minSize = { minSizeX  ,Layout::GData.heightLimit };


	assert(!x->separators.empty());


	//first
	{
		const bool haveChildren = Layout::haveChildren(x, x->separators[0]);
		if (!haveChildren) {
			window.pos = { *x->left->separator, *x->top->separator };
			window.size = { *x->right->separator - window.pos.x, *x->separators[0]->separator - window.pos.y };

			if (border != Layout::Internal::ResizedBorder::None) {
				if (Layout::GUpdateSeparator != (border == Layout::Internal::ResizedBorder::Top ? x->top : x->separators[0]))
					window.maxSize = { maxSizeX, floatMax };
				else if (border == Layout::Internal::ResizedBorder::Top)
					window.maxSize = { floatMax, window.pos.y + window.size.y - Layout::GData.heightLimit - *x->top->top };
				else if (border == Layout::Internal::ResizedBorder::Bottom)
					window.maxSize = { floatMax, *x->separators[0]->bottom - window.pos.y - Layout::GData.heightLimit };
				else
					window.maxSize = { maxSizeX, floatMax };
			}
			else
				window.maxSize = { maxSizeX, floatMax };

			Layout::round(window);
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
	}

	for (uint16_t i = 1; i < size; i++) {
		const bool haveChildren = Layout::haveChildren(x, x->separators[i]);
		if (!haveChildren) {
			window.pos = { *x->left->separator, *x->separators[i - 1]->separator };
			window.size = { *x->right->separator - window.pos.x, *x->separators[i]->separator - window.pos.y };

			if (border != Layout::Internal::ResizedBorder::None) {
				if (Layout::GUpdateSeparator != (border == Layout::Internal::ResizedBorder::Top ? x->separators[i - 1] : x->separators[i]))
					window.maxSize = { maxSizeX, floatMax };
				else if (border == Layout::Internal::ResizedBorder::Top)
					window.maxSize = { floatMax, window.pos.y + window.size.y - Layout::GData.heightLimit - *x->separators[i - 1]->top };
				else if (border == Layout::Internal::ResizedBorder::Bottom)
					window.maxSize = { floatMax, *x->separators[i]->bottom - window.pos.y - Layout::GData.heightLimit };
				else
					window.maxSize = { maxSizeX, floatMax };
			}
			else {
				window.maxSize = { maxSizeX, floatMax };
			}

			Layout::round(window);
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
	}

	//last
	{
		const bool haveChildren = Layout::haveChildren(x, nullptr);
		if (!haveChildren) {
			window.pos = { *x->left->separator, *x->separators[size - 1]->separator };
			window.size = { *x->right->separator - window.pos.x, *x->bottom->separator - window.pos.y };

			if (border != Layout::Internal::ResizedBorder::None) {
				if (Layout::GUpdateSeparator != (border == Layout::Internal::ResizedBorder::Top ? x->separators[size - 1] : x->bottom))
					window.maxSize = { maxSizeX, floatMax };
				else if (border == Layout::Internal::ResizedBorder::Top)
					window.maxSize = { floatMax, window.pos.y + window.size.y - Layout::GData.heightLimit - *x->separators[size - 1]->top };
				else if (border == Layout::Internal::ResizedBorder::Bottom)
					window.maxSize = { floatMax, *x->bottom->bottom - window.pos.y - Layout::GData.heightLimit };
				else
					window.maxSize = { maxSizeX, floatMax };
			}
			else
				window.maxSize = { maxSizeX, floatMax };

			Layout::round(window);
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
	}

}

void FD::LayoutWrite::remakeWindow_vertical(std::vector<std::shared_ptr<Layout::DockSpaceWindow>>& result, auto& x, const Layout::Internal::ResizedBorder border) {
	constexpr float floatMax = std::numeric_limits<float>::max();

	const UIF16 size = static_cast<UIF16>(x->separators.size());

	Layout::DockSpaceWindow window{};
	window.identifier = x.get();

	//limit size_y
	float maxSizeY = floatMax;
	float minSizeY = Layout::GData.heightLimit;
	if (!(x->top->separator == Layout::Internal::LayoutData::mainFrameTop && x->bottom->separator == Layout::Internal::LayoutData::mainFrameBottom)) {
		if (border == Layout::Internal::ResizedBorder::Top) {
			if (x->top == Layout::GUpdateSeparator) {
				maxSizeY = *x->bottom->separator
					- *x->top->top
					- Layout::GData.heightLimit;
				minSizeY = -*x->top->bottom
					+ *x->bottom->separator
					+ Layout::GData.heightLimit;
			}
		}
		else if (border == Layout::Internal::ResizedBorder::Bottom) {
			if (x->bottom == Layout::GUpdateSeparator) {
				maxSizeY = *x->bottom->bottom
					- *x->top->separator
					- Layout::GData.heightLimit;
				minSizeY = *x->bottom->top
					- *x->top->separator
					+ Layout::GData.heightLimit;

			}
		}
		else if (border == Layout::Internal::ResizedBorder::Left) {
			if (x->left == Layout::GUpdateSeparator) {
				minSizeY = -*x->left->right
					+ *x->right->separator
					+ Layout::GData.widthLimit;
			}
		}
		else if (border == Layout::Internal::ResizedBorder::Bottom) {
			if (x->right == Layout::GUpdateSeparator) {
				minSizeY = *x->right->left
					- *x->left->separator
					+ Layout::GData.widthLimit;
			}
		}

		if (minSizeY < Layout::GData.heightLimit)
			minSizeY = Layout::GData.heightLimit;
	}

	window.minSize = { Layout::GData.widthLimit , minSizeY };

	assert(!x->separators.empty());


	//first
	{
		const bool haveChildren = Layout::haveChildren(x, x->separators[0]);
		if (!haveChildren) {
			window.pos = { *x->left->separator, *x->top->separator };
			window.size = { *x->separators[0]->separator - window.pos.x,*x->bottom->separator - window.pos.y };

			if (border != Layout::Internal::ResizedBorder::None) {
				if (Layout::GUpdateSeparator != (border == Layout::Internal::ResizedBorder::Left ? x->left : x->separators[0]))
					window.maxSize = { floatMax, maxSizeY };
				else if (border == Layout::Internal::ResizedBorder::Left)
					window.maxSize = { window.pos.x + window.size.x - Layout::GData.widthLimit - *x->left->left, floatMax };
				else if (border == Layout::Internal::ResizedBorder::Right)
					window.maxSize = { *x->separators[0]->right - window.pos.x - Layout::GData.widthLimit ,maxSizeY };
				else
					window.maxSize = { floatMax, maxSizeY };
			}
			else
				window.maxSize = { floatMax, maxSizeY };

			Layout::round(window);
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
	}

	for (UIF16 i = 1, size = static_cast<UIF16>(x->separators.size()); i < size; i++) {
		const bool haveChildren = Layout::haveChildren(x, x->separators[i]);
		if (!haveChildren) {
			window.pos = { *x->separators[i - 1]->separator ,*x->top->separator };
			window.size = { *x->separators[i]->separator - window.pos.x,*x->bottom->separator - window.pos.y };

			if (border != Layout::Internal::ResizedBorder::None) {
				if (Layout::GUpdateSeparator != (border == Layout::Internal::ResizedBorder::Left ? x->separators[i - 1] : x->separators[i]))
					window.maxSize = { floatMax, maxSizeY };
				else if (border == Layout::Internal::ResizedBorder::Left)
					window.maxSize = { window.pos.x + window.size.x - Layout::GData.widthLimit - *x->separators[i - 1]->left, floatMax };
				else if (border == Layout::Internal::ResizedBorder::Right)
					window.maxSize = { *x->separators[i]->right - window.pos.x - Layout::GData.widthLimit,floatMax };
				else
					window.maxSize = { floatMax, maxSizeY };
			}
			else {
				window.maxSize = { floatMax, maxSizeY };
			}

			Layout::round(window);
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
	}

	//last
	{
		const bool haveChildren = Layout::haveChildren(x, nullptr);
		if (!haveChildren) {
			window.pos = { *x->separators[size - 1]->separator , *x->top->separator };
			window.size = { *x->right->separator - window.pos.x, *x->bottom->separator - window.pos.y };
			if (border != Layout::Internal::ResizedBorder::None) {
				if (Layout::GUpdateSeparator != (border == Layout::Internal::ResizedBorder::Left ? x->separators[size - 1] : x->right))
					window.maxSize = { floatMax, maxSizeY };
				else if (border == Layout::Internal::ResizedBorder::Left)
					window.maxSize = { window.pos.x + window.size.x - Layout::GData.widthLimit - *x->separators[size - 1]->left, floatMax };
				else if (border == Layout::Internal::ResizedBorder::Right)
					window.maxSize = { *x->right->right - window.pos.x - Layout::GData.widthLimit,floatMax };
				else
					window.maxSize = { floatMax, maxSizeY };
			}
			else
				window.maxSize = { floatMax, maxSizeY };

			Layout::round(window);
			result.emplace_back(std::make_shared<Layout::DockSpaceWindow>(window));
			result.back().get()->window = result.back().get();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float FD::LayoutRead::mainFrameLeft() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return *LayoutData::mainFrameLeft.get();
}

float FD::LayoutRead::mainFrameRight() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return *LayoutData::mainFrameRight.get();
}

float FD::LayoutRead::mainFrameTop() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return *LayoutData::mainFrameTop.get();
}

float FD::LayoutRead::mainFrameBottom() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return *LayoutData::mainFrameBottom.get();
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

float FD::LayoutRead::widthLimitSum() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	const std::function<void(const Layout::AdjacentInfo* const, Size&)> calc = [&](const Layout::AdjacentInfo* const child, Size& count) {

		if (!child->horizonal)
			count += child->separators.size();

		if (!child->parent)
			return;

		calc(child->parent.get(), count);
	};

	std::vector<Size> data = { 1 };

	for (auto& x : Layout::GAdjacent) {
		Size count = 1;
		if (x->haveChildren.empty())
			calc(x.get(), count);

		data.emplace_back(count);
	}
	const auto max = std::max_element(data.begin(), data.end());

	return ((*max) * Layout::GData.widthLimit) * 1.1f;
}

float FD::LayoutRead::heightLimitSum() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	const std::function<void(const Layout::AdjacentInfo* const, Size&)> calc = [&](const Layout::AdjacentInfo* const child, Size& count) {

		if (child->horizonal)
			count += child->separators.size();

		if (!child->parent)
			return;

		calc(child->parent.get(), count);
	};

	std::vector<Size> data = { 1 };

	for (auto& x : Layout::GAdjacent) {
		Size count = 1;
		if (x->haveChildren.empty())
			calc(x.get(), count);

		data.emplace_back(count);
	}
	const auto max = std::max_element(data.begin(), data.end());

	return ((*max) * Layout::GData.heightLimit) * 1.1f;
}

bool FD::LayoutRead::isLeftMost(const Layout::DockSpaceWindow& window) const noexcept {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);//std::exception

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	return layout->left->separator == LayoutData::mainFrameLeft;
}

bool FD::LayoutRead::isRightMost(const Layout::DockSpaceWindow& window) const noexcept {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);//std::exception

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	return layout->right->separator == LayoutData::mainFrameRight;
}

bool FD::LayoutRead::isTopMost(const Layout::DockSpaceWindow& window) const noexcept {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);//std::exception

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	return layout->top->separator == LayoutData::mainFrameTop;
}

bool FD::LayoutRead::isBottomMost(const Layout::DockSpaceWindow& window) const noexcept {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);//std::exception

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	return layout->bottom->separator == LayoutData::mainFrameBottom;
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

std::vector<FD::Layout::SeparatorPos> FD::LayoutRead::getSeparators() const {
	std::vector<Layout::SeparatorPos> result{};
	for (auto& x : Layout::GAdjacent) {
		if (x->horizonal) {
			for (auto& y : x->separators) {
				Layout::SeparatorPos pos{};
				pos.pos1 = { *x->left->separator ,*y->separator };
				pos.pos2 = { *x->right->separator ,*y->separator };
				pos.resize = (Layout::GUpdateSeparator == y);
				pos.horizonal = true;
				pos.identifier = y.get();

				result.emplace_back(pos);
			}
		}
		else { //vertical
			for (auto& y : x->separators) {
				Layout::SeparatorPos pos{};
				pos.pos1 = { *y->separator, *x->top->separator };
				pos.pos2 = { *y->separator, *x->bottom->separator };
				pos.resize = (Layout::GUpdateSeparator == y);
				pos.horizonal = false;
				pos.identifier = y.get();

				result.emplace_back(pos);
			}
		}

		result.emplace_back();
	}
	return result;
}

std::vector<FD::Layout::UnRedoInfo> FD::LayoutRead::getUnRedoInfo() const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);
	return LayoutData::unredo;
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

bool FD::LayoutRead::canMerge(const Layout::DockSpaceWindow& window, const ImVec2& pos) const {
	using namespace Layout::Internal;
	std::lock_guard<std::mutex> lock(LayoutData::mtx);

	Layout::AdjacentInfo* layout = static_cast<Layout::AdjacentInfo*>(window.identifier);

	std::array<std::shared_ptr<Layout::SeparatorInfo>, 4> separators{};

	if (layout->horizonal)
		separators = Layout::getSeparators(layout, pos.y).second;
	else
		separators = Layout::getSeparators(layout, pos.x).second;

	//left right top bottom
	std::array<float, 4> distance = {
		pos.x - *separators[0]->separator,
		*separators[1]->separator - pos.x,
		pos.y - *separators[2]->separator,
		*separators[3]->separator - pos.y
	};
	const auto itr = std::min_element(distance.begin(), distance.end());
	const auto min = std::distance(distance.begin(), itr);
	assert(min < 4);

	//left right
	if (min == 0 || min == 1) {
		for (auto& x : Layout::GAdjacent) {
			if (x->left == separators[min] ||
				x->right == separators[min]) {
				return false;
			}
		}
	}
	else {//top bottom
		for (auto& x : Layout::GAdjacent) {
			if (x->top == separators[min] ||
				x->bottom == separators[min]) {
				return false;
			}
		}
	}

	return true;
}

