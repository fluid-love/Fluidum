#pragma once

#include "../../Common/common.h"

namespace FS {

	class Console final : public Scene {
	public:
		explicit Console(
			FD::ConsoleWrite* const consoleWrite,
			const FD::ConsoleRead* const consoleRead
		);
		void Constructor(
			FD::ConsoleWrite,
			FD::ConsoleRead
		);

		~Console() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Console)

	public:
		virtual void call() override;

	private:
		FD::ConsoleWrite* const consoleWrite;
		const FD::ConsoleRead* const consoleRead;

	private:
		std::string inputText{};

		struct {
			const float maxFontSize = ImGui::GetFontSize() * 2.4f;
			const float minFontSize = ImGui::GetFontSize() * 0.6f;

		}limit;

		struct {
			float fontSize = ImGui::GetFontSize();
			float fontSizeScale = 1.0f;
		}style;

	private:
		void console();

	private:
		void input();

	private:
		void changeFontSize();
	};
}