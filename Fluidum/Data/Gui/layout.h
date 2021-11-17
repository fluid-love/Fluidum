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

//forward
namespace FD {
	class LayoutWrite;
	class LayoutRead;
	class ProjectWrite;
}

namespace FD::Layout {

	struct DockSpaceWindow final {
		ImVec2 pos = ImVec2();
		ImVec2 size = ImVec2();
	private:
		DockSpaceWindow* identifier = nullptr;
	private:
		friend class LayoutWrite;
	};

	namespace Internal {
		struct LayoutData final {
			FluidumUtils_Class_Delete_ConDestructor(LayoutData)
		private:
			static inline ImVec2 leftPos{};
			static inline ImVec2 rightPos{};
			static inline std::vector<std::shared_ptr<DockSpaceWindow>> windows{};

		private:
			static inline std::mutex mtx{};
			static inline std::atomic_bool save = false;
		private:
			friend class LayoutWrite;
			friend class LayoutRead;
			friend class ProjectWrite;
		};
	}
}

namespace FD {

	class LayoutWrite final {
	public:
		LayoutWrite(Internal::PassKey) {};
		~LayoutWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(LayoutWrite)

	public:
		void leftPos(const ImVec2& vec2) const;
		void rightPos(const ImVec2& vec2) const;

	public:
		void push(Layout::DockSpaceWindow& window) const;

		void splitVertical(const Layout::DockSpaceWindow& window, const float posX);
		void splitHorizonal(const Layout::DockSpaceWindow& window, const float posY);
		void splitCross(const Layout::DockSpaceWindow& window, const ImVec2& pos);


	public:
		void update(const Layout::DockSpaceWindow& window);


	public:
		void save() const noexcept;

	private:
		std::vector<std::shared_ptr<Layout::DockSpaceWindow>>::iterator find(const Layout::DockSpaceWindow& window) const;
		void checkItrEnd(const std::vector<std::shared_ptr<Layout::DockSpaceWindow>>::iterator itr) const;

		bool isRightMost(const Layout::DockSpaceWindow& window) const;
		bool isBottomMost(const Layout::DockSpaceWindow& window) const;


	};

	class LayoutRead final {
	public:
		LayoutRead(Internal::PassKey) {};
		~LayoutRead() = default;
		FluidumUtils_Class_Delete_CopyMove(LayoutRead)

	public:
		[[nodiscard]] const ImVec2& leftPos() const;
		[[nodiscard]] const ImVec2& rightPos() const;

	public:
		[[nodiscard]] Layout::DockSpaceWindow get(const uint16_t index) const;

	public:
		[[nodiscard]] bool empty() const;
		[[nodiscard]] uint16_t size() const;

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