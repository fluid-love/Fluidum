#pragma once

#include "../../Common/common.h"

namespace FS {

	class PopupBackWindow final : public Scene {
	public:
		explicit PopupBackWindow(const FD::GuiRead* const guiRead);
		void Constructor(FD::GuiRead);

		~PopupBackWindow() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::GuiRead* const guiRead;


		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

		}style;

	private:



	};
}