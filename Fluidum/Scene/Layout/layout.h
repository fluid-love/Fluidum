#pragma once

#include "../Common/common.h"

namespace FS {

	class Layout final : public Scene {
	public:
		explicit Layout(
			const FD::ImGuiWindowRead* const imguiWindowRead,
			const FD::LayoutRead* const layoutRead,
			FD::LayoutWrite* const layoutWrite,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite
		);
		void Constructor(
			FD::ImGuiWindowRead,
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
		const FD::ImGuiWindowRead* const imguiWindowRead;
		const FD::LayoutRead* const layoutRead;
		FD::LayoutWrite* const layoutWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;

		const FD::Text::Layout text{};
	private:
		std::vector<FD::Layout::DockSpaceWindow> windows{};
		std::vector<FD::Layout::SeparatorPos> separators{};
		std::vector<ImGuiID> dockSpaceIDs{};

		struct {
			FD::Layout::DockSpaceWindow* current{};
			bool focused{};
			FD::Layout::DockSpaceWindow* right{};

			UIF16 resizedWindowIndex = 0;

			FD::Layout::DockSpaceWindow* hovered{};
			FD::Layout::SeparatorPos* down = nullptr;
			ISize downIndex = -1;

			ImVec2 pos{};
		}select;

		struct {
			bool popup = false;

			bool widthConstraintArea = false;
			bool heightConstraintArea = false;
			bool centerHorizonalConstraintArea = false;
			bool centerVerticalConstraintArea = false;

			bool canMerge = false;

			bool mouseDown = false;
			bool hold = false;
		}flag;

		struct {
			ImVec2 delta{};
		}mouse;

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
		void updateWindows();
		void messageLimit();

	private:
		void focusedWindowBackground();
		void drawSeparators();
		void hoveredSeparator();
		[[nodiscard]] bool isUndockedWindowHovered();

	};
}