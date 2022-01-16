#pragma once

#include "../../Common/common.h"

namespace FD::Style {

	//single thread
	class ColorWrite final {
	public:
		explicit ColorWrite(Internal::PassKey) noexcept {};
		~ColorWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ColorWrite);

	};

	//single thread
	class ColorRead final {
	public:
		explicit ColorRead(Internal::PassKey) noexcept {};
		~ColorRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ColorRead);

	public:
		void pushButtonDisabled() const noexcept;
		void popButtonDisabled() const noexcept;

	public:
		void pushSelectableDisabled() const noexcept;
		void popSelectableDisabled() const noexcept;

	public:
		[[nodiscard]] ImVec4 createButton() const noexcept;
		[[nodiscard]] ImVec4 cancelButton() const noexcept;
		[[nodiscard]] ImVec4 backButton() const noexcept;
		[[nodiscard]] ImVec4 saveButton() const noexcept;

	public:
		[[nodiscard]] ImU32 toolBarVerticalSeparator() const noexcept;

	public:
		[[nodiscard]] ImVec4 noerror() const noexcept;

	};

}