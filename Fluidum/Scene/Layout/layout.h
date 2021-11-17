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


		struct {
			FD::Layout::DockSpaceWindow current{};
			FD::Layout::DockSpaceWindow right{};
			ImVec2 pos{};
		}select;

		struct {
			bool popup = false;

			bool limitLeft = false;
			bool isChanged = false;
		}flag;


	private:
		void dockGui();
		void dockSpace(const char* label);

	private:
		void ifRightMouseButtonCliked();
		void popup();

	private:
		void splitVerticalCurrentPos();
		void splitHorizonalCurrentPos();
		void splitCrossCurrentPos();
		void splitVerticalCenterLine();
		void splitHorizonalCenterLine();
		void splitCrossCenterLine();

	private:
		void merge();
	};
}