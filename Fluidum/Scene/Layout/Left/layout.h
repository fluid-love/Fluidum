#pragma once

#include "../../Common/common.h"

namespace FS {

	class LeftLayout final :public Scene {
	public:
		explicit LeftLayout(const FD::LayoutRead* const layoutRead, FD::LayoutWrite* const layoutWrite, const FD::GuiRead* const guiRead, FD::GuiWrite* const guiWrite);
		void Constructor(FD::LayoutRead, FD::LayoutWrite, FD::GuiRead, FD::GuiWrite);

		~LeftLayout() noexcept;

		FluidumUtils_Class_Delete_CopyMove(LeftLayout)

	public:
		virtual void call() override;

	private://data
		const FD::LayoutRead* const layoutRead;
		FD::LayoutWrite* const layoutWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;

	private://data

		struct {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();
		}style;

		struct {
			bool limitLeft = false;
			bool isChanged = false;
		}flag;

		ImGuiID id;

	private:
		void dockGui();

		void updateScissorViewport();
	};
}