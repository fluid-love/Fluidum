#pragma once

#include "../../Common/common.h"

namespace FD::Style {

	//single thread
	class ColorWrite final {
	public:
		explicit ColorWrite(Internal::PassKey) noexcept {};
		~ColorWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ColorWrite)


	};

	//single thread
	class ColorRead final {
	public:
		explicit ColorRead(Internal::PassKey) noexcept {};
		~ColorRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ColorRead)

	public:
		void pushButtonDisabled() const;
		void popButtonDisabled() const;


	};




}