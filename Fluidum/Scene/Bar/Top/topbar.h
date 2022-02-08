#pragma once

#include "../../Common/common.h"

namespace FS {

	class TopBar final : public Scene {
	public:
		explicit TopBar(
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::SceneRead* const sceneRead,
			const FD::ToolBarRead* const toolBarRead,
			FD::ToolBarWrite* const toolBarWrite,
			const FD::ImGuiWindowRead* const imguiWindowRead,
			const FD::LayoutRead* const layoutRead,
			FD::ConsoleWrite* const conoleWrite
		);
		void Constructor(
			FD::ProjectRead,
			FD::GuiRead,
			FD::GuiWrite,
			FD::SceneRead,
			FD::ToolBarRead,
			FD::ToolBarWrite,
			FD::ImGuiWindowRead,
			FD::LayoutRead,
			FD::ConsoleWrite
		);

		~TopBar() noexcept;

		FluidumUtils_Class_Delete_CopyMove(TopBar);

	public:
		virtual void call() override;

	private:
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;
		const FD::SceneRead* const sceneRead;
		const FD::ToolBarRead* const toolBarRead;
		FD::ToolBarWrite* const toolBarWrite;
		const FD::ImGuiWindowRead* const imguiWindowRead;
		const FD::LayoutRead* const layoutRead;
		FD::ConsoleWrite* const consoleWrite;

		FD::Text::TopBar text{};

	private:
		struct {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();

			const ImVec4 disableButtonColor = { 0.02f,0.02f,0.02f,0.4f };
		}style;

		struct {
			bool calc = false;

		}state;

		struct {
			ImVec2 run{};
		}pos;

		struct {
			ImCounter<ImAnimeTime> run{};

		}anime;

	private:
		void updateStyle();

	private:
		void rightGui();

		void areaGui();

		void calc();
		void mode();
		void run();

		void playCheck();

	private://left
		void scene();
		void combo();
		void func();

	private:
		void separator(const float posX, const ImVec4& col4 = { 0.2f, 0.2f, 0.2f, 1.0f });
		void windowBorder();

	};

}