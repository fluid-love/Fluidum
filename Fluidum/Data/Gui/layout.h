#pragma once

#include "../Common/common.h"

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
		friend class LayoutRead;
	};

	struct SeparatorPos final {
		ImVec2 pos1{};
		ImVec2 pos2{};
		bool resize = false;
	};

	struct UnRedoInfo final {
		enum class Type :uint8_t {
			Resize,
			Merge,
			Split
		};
		Type type{};

		//resize
		float pos1{};
		float pos2{};

	private:
		float* separator = nullptr;
		float calcMidpoint(const float first, const float last) const;
	private:
		friend class LayoutWrite;
	};

	namespace Internal {


		struct History final {
			float* pos = nullptr;

			float* pos_side1 = nullptr;
			float* pos_side2 = nullptr;

			bool horizonal = false;
			ImVec2 readPosRatio{};
		};

		enum class ResizedBorder : uint8_t {
			None,
			Left,
			Right,
			Top,
			Bottom
		};

		struct LayoutData final {
			FluidumUtils_Class_Delete_ConDestructor(LayoutData);
		private:
			static inline std::shared_ptr<float> mainFrameLeft{};
			static inline std::shared_ptr<float> mainFrameRight{};
			static inline std::shared_ptr<float> mainFrameTop{};
			static inline std::shared_ptr<float> mainFrameBottom{};
			static inline std::vector<std::shared_ptr<DockSpaceWindow>> windows{};

			static inline std::vector<UnRedoInfo> unredo{};
			static inline std::size_t currentUnRedoIndex = 0;

			static inline std::vector<History> history{};
		private:
			static inline std::mutex mtx{};
			static inline std::atomic_bool save = false;
		private:
			static void remake();
			static DockSpaceWindow* findWindow(const ImVec2& pos);
		private:
			friend class LayoutWrite;
			friend class LayoutRead;
			friend class ProjectWrite;
		};
	}

	using ResizedBorder = Internal::ResizedBorder;
}

namespace FD {

	class LayoutWrite final {
	public:
		LayoutWrite(Internal::PassKey) {};
		~LayoutWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(LayoutWrite);

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
		static void reset();
		
		//Remake(rebuild) the layout from history.
		void remake();

		static bool splitVertical(const Layout::DockSpaceWindow& window, const float posX);
		static bool splitHorizonal(const Layout::DockSpaceWindow& window, const float posY);

	public:
		bool merge(const Layout::DockSpaceWindow& window, const ImVec2& pos) const;

	public:
		Layout::ResizedBorder update(const Layout::DockSpaceWindow& window, const ImVec2& mousePos, Layout::ResizedBorder border) const;
		void resize(const Layout::DockSpaceWindow& window, const Layout::ResizedBorder border) const;

	public:
		void undo(const std::size_t count) const;
		void redo(const std::size_t count) const;

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
		FluidumUtils_Class_Delete_CopyMove(LayoutRead);

	public:
		[[nodiscard]] float mainFrameLeft() const;
		[[nodiscard]] float mainFrameRight() const;
		[[nodiscard]] float mainFrameTop() const;
		[[nodiscard]] float mainFrameBottom() const;

		[[nodiscard]] float widthLimit() const;
		[[nodiscard]] float heightLimit() const;

	public:
		[[nodiscard]] std::vector<Layout::DockSpaceWindow> get() const;

		[[nodiscard]] std::vector<Layout::SeparatorPos> getSeparators() const;

		[[nodiscard]] std::vector<Layout::UnRedoInfo> getUnRedoInfo() const;

	public:
		[[nodiscard]] bool empty() const;
		[[nodiscard]] uint16_t size() const;

	public:
		[[nodiscard]] bool canMerge(const Layout::DockSpaceWindow& window, const ImVec2& pos) const;

	};
}