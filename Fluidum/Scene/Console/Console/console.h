#pragma once

#include "../../Common/common.h"

namespace FS {

	class Console final : public Scene {
	public:
		explicit Console(
			FD::ConsoleWrite* const consoleWrite,
			const FD::ConsoleRead* const consoleRead,
			const FD::Style::VarRead* const varRead,
			FD::ToolBarWrite* const toolBarWrite
		);
		void Constructor(
			FD::ConsoleWrite,
			FD::ConsoleRead,
			FD::Style::VarRead,
			FD::ToolBarWrite
		);

		~Console() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Console);

	public:
		virtual void call() override;

	private:
		FD::ConsoleWrite* const consoleWrite;
		const FD::ConsoleRead* const consoleRead;
		const FD::Style::VarRead* const varRead;
		FD::ToolBarWrite* const toolBarWrite;

		const FD::Text::Console text{};

	private:
		std::string inputText{};

		struct {
			const float maxFontSize = ImGui::GetFontSize() * 2.4f;
			const float minFontSize = ImGui::GetFontSize() * 0.6f;

		}limit;

		struct {
			float fontSize = ImGui::GetFontSize();
			float fontSizeScale = 1.0f;

			float inputTextWindowHeight{};
		}style;

		struct {
			bool windowFlag = true;
			bool popupTitle = false;
			bool popupRight = false;
		}flag;

	private:
		void toolBar();

	private:
		void closeWindow();

	private:
		void console();

	private:
		void input();
		void push();

	private:
		void popupTitle();

		void popupRight();
		void popup_clear();
		void popup_backcolor();

	private:
		void changeFontSize();

	};

}