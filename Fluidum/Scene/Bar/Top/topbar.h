#pragma once

#include "../../Common/common.h"

namespace FS {

	class TopBar final :public Scene {
	public:
		explicit TopBar(
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::SceneRead* const sceneRead
		);
		void Constructor(FD::ProjectRead, FD::GuiRead, FD::GuiWrite, FD::SceneRead);

		~TopBar() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite = nullptr;
		const FD::SceneRead* const sceneRead = nullptr;


	private://data

		struct {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();

			ImVec2 packageWindowPos = ImVec2();
			ImVec2 packageWindowSize = ImVec2();

			const ImVec4 disableButtonColor = { 0.02f,0.02f,0.02f,0.4f };
		}style;

		struct {
			bool calc = false;

		}state;

		FD::Text::TopBar text{};

		//struct {
		//	void reset() {
		//		isPlayButtonClicked = false;
		//		time = std::chrono::system_clock::time_point();
		//	}
		//	bool isPlayButtonClicked = false;
		//	std::chrono::system_clock::time_point time{};
		//}wait;
		//FS::Parameter::Loading<> loadingParam = {};

	private:
		void packageGui();

		void layoutGui();

		void templateGui();

		void projectNameGui();

		void rightGui();
		void backColorGui();
		void areaGui();

		void calc();
		void run();

		void playCheck();

		void file();
	};
}