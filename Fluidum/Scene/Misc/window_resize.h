#pragma once

#include "../Common/common.h"

namespace FS::Misc {

	class ResizeWindow final : public Scene {
	public:
		explicit ResizeWindow(
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::Style::ColorRead* const colorRead
		);
		void Constructor(
			FD::GuiRead,
			FD::GuiWrite,
			FD::Style::ColorRead
		);

		~ResizeWindow() noexcept;

		FluidumUtils_Class_Delete_CopyMove(ResizeWindow);

	private:
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;
		const FD::Style::ColorRead* const colorRead;

	private:
		enum class Border {
			None,
			Left,
			Right,
			Top,
			Bottom,
			Move
		};

		Border currentBorder = Border::None;
		Border holdBorder = Border::None;

		IF32 mousePosX{};
		IF32 mousePosY{};
		IF32 windowPosX{};
		IF32 windowPosY{};
		IF32 windowSizeX{};
		IF32 windowSizeY{};

		IF32 deltaX{};
		IF32 deltaY{};
		IF32 dragDeltaX{};
		IF32 dragDeltaY{};

		bool down{};
		bool isItemHoved{};

		ImVec2 viewPortPos{};

		Border limit = Border::None;


		struct {
			const std::array<float, 4> chromaKey{ 0.0f,1.0f,0.0f,1.0f };
			const std::array<I32, 3> chromaKeyByte{ 0, 255, 0 };
		}color;

	public:
		virtual void call() override;

	private:
		void background();

	private:
		void leftCursor();
		void rightCursor();
		void topCursor();
		void bottomCursor();

	private:
		void resize_left();
		void resize_right();
		void resize_top();
		void resize_bottom();

		void hold();

	private:
		void moveWindow();

	private:
		void decorateWindow();
		void decorateWindow_frame();
		void decorateWindow_limitBorder();
		void decorateWindow_resizingBorder();
		void setLimitNone();
		[[nodiscard]] std::pair<ImVec2, ImVec2> borderPos(const Border border);

	private:
		void updateGuiData();

	};

}