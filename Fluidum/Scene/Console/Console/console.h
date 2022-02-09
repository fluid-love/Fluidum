#pragma once

#include "../../Common/common.h"

namespace FS {

	class Console final : public Scene {
	public:
		explicit Console(
			FD::ImGuiWindowWrite* const imguiWindowWrite,
			FD::ConsoleWrite* const consoleWrite,
			const FD::ConsoleRead* const consoleRead,
			const FD::Style::ColorRead* const colorRead,
			const FD::Style::VarRead* const varRead,
			FD::ToolBarWrite* const toolBarWrite
		);
		void Constructor(
			FD::ImGuiWindowWrite,
			FD::ConsoleWrite,
			FD::ConsoleRead,
			FD::Style::ColorRead,
			FD::Style::VarRead,
			FD::ToolBarWrite
		);

		~Console() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Console);

	public:
		virtual void call() override;

	private:
		FD::ImGuiWindowWrite* const imguiWindowWrite;
		FD::ConsoleWrite* const consoleWrite;
		const FD::ConsoleRead* const consoleRead;
		const FD::Style::ColorRead* const colorRead;
		const FD::Style::VarRead* const varRead;
		FD::ToolBarWrite* const toolBarWrite;

		const FD::Text::Console text{};
		struct {
			FD::Text::Common collapseWindow{ FD::Text::CommonText::CollapseWindow };
		}text_;

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
			bool collapseWindow = false;
			bool isWindowCollpsed = false;
			bool popupTitle = false;
			bool popupRight = false;
		}flag;

	private:
		void setImGuiWindow();

	private:
		void toolBar();

	private:
		void closeWindow();

	private:
		void console();
		[[nodiscard]] ImVec4 messageColor(const FU::Log::Type type) const;

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