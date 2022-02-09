#pragma once

#include "../../Common/common.h"

namespace FD::Style {

	class ThemeWrite final {
	public:
		explicit ThemeWrite(Internal::PassKey) noexcept {};
		~ThemeWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ThemeWrite);

	public:
		void setTheme() noexcept;
		void setDefault() noexcept;

	};

	class ThemeRead final {
	public:
		explicit ThemeRead(Internal::PassKey) noexcept {};
		~ThemeRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ThemeRead);

	public:


	};

}