#pragma once

#include "../../Common/common.h"

namespace FS {

	class StatusBar final :public Scene {
	public:
		explicit StatusBar(const FD::GuiRead* const guiRead);
		void Constructor(FD::GuiRead);

		~StatusBar() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::GuiRead* const guiRead;

		FD::Text::StatusBar text{};

	private://data

		struct Size final {
			float taskIcon = 0.0f;
			float version = 0.0f;

			float barHeight = 30.0f;
			ImVec2 windowPos = ImVec2();

			ImVec2 infoWindowPos = ImVec2();
			ImVec2 infoWindowSize = ImVec2();

			ImVec2 versionWindowPos = ImVec2();
			ImVec2 versionWindowSize = ImVec2();
		}style;


	private:
		//左下に今のタスク
		void taskGui();

		//中央
		void infoGui();

		//右下のバージョン
		void versionGui();

	};
}