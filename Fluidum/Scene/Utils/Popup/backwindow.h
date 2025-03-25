#pragma once

#include "../../Common/common.h"

namespace FS::Utils {

	class PopupBackWindow final : public Scene {
	public:
		explicit PopupBackWindow(const FD::GuiRead* const guiRead);
		void Constructor(FD::GuiRead);

		~PopupBackWindow() noexcept;

		FluidumUtils_Class_Delete_CopyMove(PopupBackWindow);

	public:
		virtual void call() override;

	private:
		const FD::GuiRead* const guiRead;

	private:
		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

		}style;
	
	};

}