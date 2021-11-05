#pragma once

#include "../../Common/common.h"

namespace FS {

	class TopBar final :public Scene {
	public:
		explicit TopBar(
			const FD::ProjectRead* const projectRead,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::SceneRead* const sceneRead,
			const FD::TopBarRead* const topBarRead,
			FD::TopBarWrite* const topBarWrite
		);
		void Constructor(
			FD::ProjectRead,
			FD::FluidumFilesRead,
			FD::GuiRead,
			FD::GuiWrite,
			FD::SceneRead,
			FD::TopBarRead,
			FD::TopBarWrite
		);

		~TopBar() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;
		const FD::SceneRead* const sceneRead;
		const FD::TopBarRead* const topBarRead;
		FD::TopBarWrite* const topBarWrite;

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

		struct {
			ImVec2 run{};
		}pos;

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

	private:
		void templateGui();
		void setTemplate(const uint16_t index);

	private:
		void projectNameGui();

		void rightGui();
		void backColorGui();
		void areaGui();

		void calc();
		void run();

		void playCheck();

	private://left
		void scene();
		void combo();
		void func();

	private:
		void separator(const float posX, const ImVec4& col4 = { 0.2f, 0.2f, 0.2f, 1.0f });
	};
}