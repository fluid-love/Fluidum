#pragma once

#include "../../Common/common.h"

namespace FS::Coding {

	class Tab final : public Scene {
	public:
		explicit Tab(
			FD::ImGuiWindowWrite* const imguiWindowWrite,
			FD::Coding::TabWrite* const tabWrite,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::DisplayWrite* const displayWrite,
			const FD::Coding::DisplayRead* const displayRead,
			const FD::ProjectRead* const projectRead,
			const FD::LayoutRead* const layoutRead,
			FD::ToolBarWrite* const toolBarWrite,
			const FD::Style::ColorRead* const colorRead
		);
		void Constructor(
			FD::ImGuiWindowWrite,
			FD::Coding::TabWrite,
			FD::Coding::TabRead,
			FD::Coding::DisplayWrite,
			FD::Coding::DisplayRead,
			FD::ProjectRead,
			FD::LayoutRead,
			FD::ToolBarWrite,
			FD::Style::ColorRead
		);

		~Tab() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Tab);

	public:
		virtual void call() override;

	private:
		FD::ImGuiWindowWrite* const imguiWindowWrite;
		FD::Coding::TabWrite* const tabWrite;
		const FD::Coding::TabRead* const tabRead;
		const FD::ProjectRead* const projectRead;
		FD::Coding::DisplayWrite* const displayWrite;
		const FD::Coding::DisplayRead* const displayRead;
		FD::ToolBarWrite* const toolBarWrite;
		const FD::Style::ColorRead* const colorRead;

		FD::Text::CodingTab text{};

	private:
		bool windowCloseFlag = true;

		struct {
			ImVec2 windowMinSize{};
		}style;

		struct {
			ImVec2 center{};
			ImVec2 clicked{};
			ImVec2 toolBar_saveSelected{};
			ImVec2 toolBar_saveAll{};
		}pos;

		struct {
			ImCounter<ImAnimeTime> toolBar_saveSelected{};
			ImCounter<ImAnimeTime> toolBar_saveAll{};
		}anime;

		struct File final {
			std::string path;//full

			// "C:/test/text.lua" -> "text.lua"
			std::string name;

			//!isTextSaved
			bool asterisk = false;
		};

		struct {
			std::vector<File> files{};
		}info;

		struct {
			UIF16 index = 0;
			IF32 hovered = -1;
		}select;

		/*
		When an item is deleted from a tab,
		the previously selected item will be selected.
		*/
		struct Recent final {
		public:
			using Index = std::remove_const_t<decltype(FD::Coding::Tab::Limits::FileSizeMax)>;

		public:
			inline void push(const Index index) {
				if (this->history.size() >= FD::Coding::Tab::Limits::RecentDepthMax)
					this->history.pop_front();
				this->history.emplace_back(index);
			}

			[[nodiscard]] inline Index back() const noexcept {
				return this->history.back();
			}

			[[nodiscard]] inline bool empty() const noexcept {
				return this->history.empty();
			}

			inline void sort(const Index index) noexcept {
				for (auto& x : this->history) {
					if (x >= index)
						x++;
				}
			}

			inline void erase(const Index index) {
				std::erase(history, index);
				for (Index& x : this->history) {
					if (x > index)
						x--;
				}
			}

			inline void reset() noexcept {
				this->history.clear();
			}

		private:
			std::deque<Index> history{};

		}recent;

	private://misc
		void checkWindowShouldClose();
		void setImGuiWindow();

	private://toolbar
		void toolBar();
		void toolBar_dummy();
		void toolBar_saveSelected();
		void toolBar_saveAll();

	private:
		void update();
		void updateInfo_add();
		void updateInfo_remove();
		void updateTextSaved();

	private:
		void fileList();

	private:
		void display();
		//no-throw
		[[nodiscard]] bool addDisplayPath(const std::string& path) noexcept;
		[[nodiscard]] bool removeDisplayPath(const std::string& path) noexcept;
		[[nodiscard]] bool checkExistsFile(const std::string& path) noexcept;


	private:
		void closeButton();
		void close();

	private:
		void popup();
		void popup_window();

	};

}