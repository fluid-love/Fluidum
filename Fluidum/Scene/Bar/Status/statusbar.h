#pragma once

#include "../../Common/common.h"

namespace FS {

	class StatusBar final :public Scene {
	public:
		explicit StatusBar(
			FD::GuiWrite* const guiWrite,
			const FD::GuiRead* const guiRead,
			const FD::TaskRead* const taskRead
		);
		void Constructor(
			FD::GuiWrite,
			FD::GuiRead, 
			FD::TaskRead
		);

		~StatusBar() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::GuiRead* const guiRead;
		const FD::TaskRead* const taskRead;

		FD::Text::StatusBar text{};

	private://data

		struct Size final {
			float taskIcon{};
			float version{};

			float barHeight{};
			ImVec2 windowPos{};

			ImVec2 infoWindowPos{};
			ImVec2 infoWindowSize{};

			ImVec2 versionWindowPos{};
			ImVec2 versionWindowSize{};
		}style;


	private:
		//左下に今のタスク
		void taskGui();
		void taskPopup();

		//中央
		void infoGui();

		//右下のバージョン
		void versionGui();

	};
}