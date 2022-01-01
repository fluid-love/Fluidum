#pragma once

#include "../../Common/common.h"

namespace FD::Style {

	//single thread
	class VarWrite final {
	public:
		explicit VarWrite(Internal::PassKey) noexcept {};
		~VarWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(VarWrite)

	public:
		//Title Scene
		void initialize() const;
	};

	//single thread
	class VarRead final {
	public:
		explicit VarRead(Internal::PassKey) noexcept {};
		~VarRead() = default;
		FluidumUtils_Class_Delete_CopyMove(VarRead)

	public:
		[[nodiscard]] float iconDummyWidth() const;

		[[nodiscard]] void iconDummy() const;


	};




}