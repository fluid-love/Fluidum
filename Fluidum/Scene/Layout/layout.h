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

		FluidumUtils_Class_Delete_CopyMove(Layout)

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

		struct {
			FD::Layout::DockSpaceWindow current{};
			FD::Layout::DockSpaceWindow right{};
			FD::Layout::DockSpaceWindow resize{};

			ImVec2 pos{};
		}select;

		struct {
			bool popup = false;

			bool widthConstraintArea = false;
			bool heightConstraintArea = false;
			bool centerHorizonalConstraintArea = false;
			bool centerVerticalConstraintArea = false;
		}flag;


	private:
		void dockGui();
		void dockSpace(const char* label);

	private:
		void ifRightMouseButtonCliked();
		void popup();
		bool widthConstraintArea(const ImVec2& mousePos);
		bool heightConstraintArea(const ImVec2& mousePos);
		bool centerHorizonalConstraintArea();
		bool centerVerticalConstraintArea();
	private:
		void splitVerticalCurrentPos();
		void splitHorizonalCurrentPos();
		void splitCrossCurrentPos();
		void splitVerticalCenterLine();
		void splitHorizonalCenterLine();
		void splitCrossCenterLine();
		void reset();

	private:
		void merge();

	};
}