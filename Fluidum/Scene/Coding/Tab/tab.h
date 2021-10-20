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


		bool windowCloseFlag = false;

		struct {
			ImVec2 topBarSize{};
		}style;

		struct {
			ImVec2 center{};
		}pos;

		struct {
			std::vector<std::string> pathes;//full

			// "C:/test/text.lua" -> "text.lua"
			std::vector<std::string> fileNames;

			std::size_t currentIndex = 0;
		}files;

		struct {
			uint16_t index = 0;
		}select;

	private:
		void checkWindowShouldClose();


	private:
		void update();

	private:
		void fileList();
		void display();
	};
}