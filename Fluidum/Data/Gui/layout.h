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
		ImVec2 pos{};
		ImVec2 size{};
		ImVec2 minSize{};
		ImVec2 maxSize{};
	private:
		void* identifier = nullptr; //::FD::Layout::AdjacentInfo
		DockSpaceWindow* window = nullptr;

	private:
		friend class LayoutWrite;
	};

	namespace Internal {
		struct History final {
			float* pos = nullptr;
			float readPos{};
			uint32_t parentWindowIndex = 0;
			bool horizonal = false;
		};

		enum class ResizedBorder : uint8_t {
			None,
			Left,
			Right,
			Top,
			Bottom
		};

		struct LayoutData final {
			FluidumUtils_Class_Delete_ConDestructor(LayoutData)
		private:
			static inline float mainFrameLeft{};
			static inline float mainFrameRight{};
			static inline float mainFrameTop{};
			static inline float mainFrameBottom{};
			static inline std::vector<std::shared_ptr<DockSpaceWindow>> windows{};

			static inline std::vector<History> history{};
		private:
			static inline std::mutex mtx{};
			static inline std::atomic_bool save = false;
		private:
			static void remake();
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
		void mainFrameLeft(const float val) const;
		void mainFrameRight(const float val) const;
		void mainFrameTop(const float val) const;
		void mainFrameBottom(const float val) const;

	public:
		void widthLimit(const float val) const;
		void heightLimit(const float val) const;

	public:
		//construct main layout window from LayoutData
		void reset() const;

		static bool splitVertical(const Layout::DockSpaceWindow& window, const float posX);
		static bool splitHorizonal(const Layout::DockSpaceWindow& window, const float posY);

	public:
		void merge() const;

	public:
		void update(const Layout::DockSpaceWindow& window) const;

	public:
		void save() const noexcept;

	private:
		static void remakeAllWindows(const Layout::Internal::ResizedBorder border = Layout::Internal::ResizedBorder::None);
		static void remakeWindow_horizonal(std::vector<std::shared_ptr<Layout::DockSpaceWindow>>& result, auto& x, const Layout::Internal::ResizedBorder border);
		static void remakeWindow_vertical(std::vector<std::shared_ptr<Layout::DockSpaceWindow>>& result, auto& x, const Layout::Internal::ResizedBorder border);

	private:
		friend Layout::Internal::LayoutData;
	};

	class LayoutRead final {
	public:
		LayoutRead(Internal::PassKey) {};
		~LayoutRead() = default;
		FluidumUtils_Class_Delete_CopyMove(LayoutRead)

	public:
		[[nodiscard]] float mainFrameLeft() const;
		[[nodiscard]] float mainFrameRight() const;
		[[nodiscard]] float mainFrameTop() const;
		[[nodiscard]] float mainFrameBottom() const;

		[[nodiscard]] float widthLimit() const;
		[[nodiscard]] float heightLimit() const;

	public:
		[[nodiscard]] std::vector<Layout::DockSpaceWindow> get() const;

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