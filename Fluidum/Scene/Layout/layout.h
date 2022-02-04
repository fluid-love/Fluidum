#pragma once

#include "../Common/common.h"

namespace FS {

	class Layout final : public Scene {
	public:
		explicit Layout(
			const FD::LayoutRead* const layoutRead,
			FD::LayoutWrite* const layoutWrite,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite
		);
		void Constructor(
			FD::LayoutRead,
			FD::LayoutWrite,
			FD::GuiRead,
			FD::GuiWrite
		);

		~Layout() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Layout);

	public:
		virtual void call() override;

	private:
		const FD::LayoutRead* const layoutRead;
		FD::LayoutWrite* const layoutWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;

		const FD::Text::Layout text{};
	private:
		std::vector<FD::Layout::DockSpaceWindow> windows{};
		std::vector<FD::Layout::SeparatorPos> separators{};

		struct {
			FD::Layout::DockSpaceWindow* current{};
			bool focused{};
			FD::Layout::DockSpaceWindow* right{};

			UIF16 resizedWindowIndex = 0;

			FD::Layout::DockSpaceWindow* hovered{};

			FD::Layout::ResizedBorder resizeBorder = FD::Layout::ResizedBorder::None;

			ImVec2 pos{};
		}select;

		struct {
			bool popup = false;

			bool widthConstraintArea = false;
			bool heightConstraintArea = false;
			bool centerHorizonalConstraintArea = false;
			bool centerVerticalConstraintArea = false;

			bool noresize = false;

			bool canMerge = false;

			bool mouseDown = false;
		}flag;

	private:
		void updateLayout();
		void updateWindowMinSize();

	private:
		void dockGui();
		void dockSpace(const char* label);

	private:
		void ifRightMouseButtonCliked();
		void popup();
		bool widthConstraintArea();
		bool heightConstraintArea();
		bool centerHorizonalConstraintArea();
		bool centerVerticalConstraintArea();
	private:
		void splitVerticalCurrentPos();
		void splitHorizonalCurrentPos();
		void splitVerticalCenterLine();
		void splitHorizonalCenterLine();
		void reset();

	private:
		void merge();

	private:
		void noresize();
		void save_resize();

	private:
		void updateWindows();
		void messageLimit();

	private:
		void focusedWindowBackground();
		void drawSeparators();
		void hoveredSeparator();

	};
}