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

		Border limit = Border::None;

	public:
		virtual void call() override;

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
		void decorateWindow_border();
		void setLimitNone();

	private:
		void updateGuiData();

	};

}