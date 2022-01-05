#pragma once

#include "../Common/common.h"

namespace FD {

	//single thread
	class GuiWrite final {
	public:
		GuiWrite(Internal::PassKey) {};
		~GuiWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(GuiWrite)

	public:
		void centerPos(const ImVec2& vec2) const noexcept;
		void windowSize(const ImVec2& vec2) const noexcept;
		void leftBarWidth(const float width) const noexcept;
		void menuBarHeight(const float height) const noexcept;
		void topBarHeight(const float height) const noexcept;
		void statusBarHeight(const float height) const noexcept;

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
		float statusBarHeight() const noexcept;



	};
}

namespace FD {

	class ImGuiWindowWrite final {
	public:
		ImGuiWindowWrite(Internal::PassKey) {};
		~ImGuiWindowWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ImGuiWindowWrite)

	public:
		//require: std::same_as<T, ImGuiWindow*>
		//avoid #include <imgui_internal.h>
		template<typename T>
		void set(const FU::Class::ClassCode::CodeType classCode, T windowPtr) const;

	};

	class ImGuiWindowRead final {
	public:
		ImGuiWindowRead(Internal::PassKey) {};
		~ImGuiWindowRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ImGuiWindowRead)

	public:
		//return ImGuiWindow*
		template<typename T>
		[[nodiscard]] T get(const FU::Class::ClassCode::CodeType classCode) const;

	};

}