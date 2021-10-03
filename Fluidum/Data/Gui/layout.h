#pragma once

#include "../Common/common.h"

namespace FD {

	class GuiWrite final {
	public:
		GuiWrite(Internal::PassKey) {};
		~GuiWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(GuiWrite)

#ifdef FluidumData_DebugMode
			GuiWrite() = default;
#endif 

	public:
		void centerPos(const ImVec2& vec2) const noexcept;
		void windowSize(const ImVec2& vec2) const noexcept;
		void leftBarWidth(const float width) const noexcept;
		void menuBarHeight(const float height) const noexcept;
		void topBarHeight(const float height) const noexcept;


	};

	class GuiRead final {
	public:
		GuiRead(Internal::PassKey) {};
		~GuiRead() = default;
		FluidumUtils_Class_Delete_CopyMove(GuiRead)

	public:
		const ImVec2& centerPos() const noexcept;
		const ImVec2& windowSize() const noexcept;
		float leftBarWidth() const noexcept;
		float menuBarHeight() const noexcept;
		float topBarHeight() const noexcept;



	};

	class LayoutWrite final {
	public:
		LayoutWrite(Internal::PassKey) {};
		~LayoutWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(LayoutWrite)

#ifdef FluidumData_DebugMode
			LayoutWrite() = default;
#endif 

	public:
		void leftLayoutPos(const ImVec2& vec2) const noexcept;
		void leftLayoutSize(const ImVec2& vec2) const noexcept;
		void rightLayoutPos(const ImVec2& vec2) const noexcept;
		void rightLayoutSize(const ImVec2& vec2) const noexcept;


	};

	class LayoutRead final {
	public:
		LayoutRead(Internal::PassKey) {};
		~LayoutRead() = default;
		FluidumUtils_Class_Delete_CopyMove(LayoutRead)

	public:
		_NODISCARD ImVec2 leftLayoutPos() const noexcept;
		_NODISCARD ImVec2 leftLayoutSize() const noexcept;
		_NODISCARD ImVec2 rightLayoutPos() const noexcept;
		_NODISCARD ImVec2 rightLayoutSize() const noexcept;


	};




}