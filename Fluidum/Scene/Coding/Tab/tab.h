#pragma once

#include "../../Common/common.h"

namespace FS::Coding {

	class Tab final : public Scene {
	public:
		explicit Tab(
			FD::Coding::TabWrite* const tabWrite,
			const FD::Coding::TabRead* const tabRead,
			const FD::ProjectRead* const projectRead
		);
		void Constructor(FD::Coding::TabWrite, FD::Coding::TabRead, FD::ProjectRead);

		~Tab() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Tab)

	public:
		virtual void call() override;

	private://data
		FD::Coding::TabWrite* const tabWrite;
		const FD::Coding::TabRead* const tabRead;
		const FD::ProjectRead* const projectRead;

		FD::Text::CodingTab text{};

	private:
		bool windowCloseFlag = false;

		struct {
			ImVec2 topBarSize{};
		}style;

		struct {
			ImVec2 center{};
		}pos;

		struct File final {
			std::string path;//full

			// "C:/test/text.lua" -> "text.lua"
			std::string name;

			//!isTextSaved
			bool asterisk = false;
		};

		struct {
			std::vector<File> files{};
			std::size_t currentIndex = 0;
		}info;

		struct {
			uint16_t index = 0;
			int32_t hovered = -1;
		}select;

	private:
		void checkWindowShouldClose();


	private:
		void update();
		void updateInfo();
		void updateTextSaved();

	private:
		void fileList();
		void display();

		void closeButton();
		void close();

	private:
		void popup();
		void popup_window();

	};
}