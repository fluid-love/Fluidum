#pragma once

#include "../Common/common.h"

namespace FD {

	//single thread
	class GuiWrite final {
	public:
		GuiWrite(Internal::PassKey) {};
		~GuiWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(GuiWrite);

	public:
		void windowPos(const ImVec2& vec2) noexcept;
		void centerPos(const ImVec2& vec2) noexcept;
		void windowSize(const ImVec2& vec2) noexcept;

	public:
		void leftBarWidth(const float width) noexcept;
		void menuBarHeight(const float height) noexcept;
		void topBarHeight(const float height) noexcept;
		void statusBarHeight(const float height) noexcept;

	public:
		void titleBarLeft(const float left) noexcept;

	public:
		void windowLimitMinWidth(const float width) noexcept;
		void windowLimitMinHeight(const float height) noexcept;

	};

}

namespace FD {

	//single thread
	class GuiRead final {
	public:
		GuiRead(Internal::PassKey) {};
		~GuiRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(GuiRead);

	public:
		[[nodiscard]] const ImVec2& windowPos() const noexcept;
		[[nodiscard]] const ImVec2& centerPos() const noexcept;
		[[nodiscard]] const ImVec2& windowSize() const noexcept;

	public:
		[[nodiscard]] float leftBarWidth() const noexcept;
		[[nodiscard]] float menuBarHeight() const noexcept;
		[[nodiscard]] float topBarHeight() const noexcept;
		[[nodiscard]] float statusBarHeight() const noexcept;

	public:
		[[nodiscard]] float titleBarLeft() const noexcept;

	public:
		[[nodiscard]] float windowLimitMinWidth() const noexcept;
		[[nodiscard]] float windowLimitMinHeight() const noexcept;

	};

}

namespace FD {

	class ImGuiWindowWrite final {
	public:
		ImGuiWindowWrite(Internal::PassKey) {};
		~ImGuiWindowWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ImGuiWindowWrite);

	public:
		//require: std::same_as<T, ImGuiWindow*>
		//avoid #include <imgui_internal.h>
		template<typename T>
		void set(const FU::Class::ClassCode::CodeType classCode, T windowPtr) const;

	};

}

namespace FD {

	class ImGuiWindowRead final {
	public:
		ImGuiWindowRead(Internal::PassKey) {};
		~ImGuiWindowRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(ImGuiWindowRead);

	public:
		//return ImGuiWindow*
		template<typename T>
		[[nodiscard]] T get(const FU::Class::ClassCode::CodeType classCode) const;

	};

}