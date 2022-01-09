#pragma once

#include "../../Common/common.h"

namespace FD::Style {

	//single thread
	class VarWrite final {
	public:
		explicit VarWrite(Internal::PassKey) noexcept {};
		~VarWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(VarWrite);

	public:
		//Title Scene
		void initialize();
	};

	//single thread
	class VarRead final {
	public:
		explicit VarRead(Internal::PassKey) noexcept {};
		~VarRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(VarRead);

	public:
		[[nodiscard]] float iconDummyWidth() const noexcept;

		//Insert a dummy equal to the width of the icon.
		void iconDummy() const;

	public:
		[[nodiscard]] float inputTextHeight() const noexcept;

	public:
		[[nodiscard]] ImVec2 viewWindowSizeConstraints() const noexcept;

	public://popup window
		[[nodiscard]] float popupWindowBorderSize() const noexcept;

		[[nodiscard]] static consteval ImGuiWindowFlags PopupWindowFlags() {
			return
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_NoTitleBar;
		}

	};

}