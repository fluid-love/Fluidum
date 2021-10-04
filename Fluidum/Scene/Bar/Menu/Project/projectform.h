/*
MenuBar->êVãKçÏê¨->ïKóvÇ»èÓïÒÇì¸ÇÍÇÈ
*/

#pragma once

#include "../../../Common/common.h"

namespace FS::Bar {

	class ProjectForm final : public Scene {
	public:
		explicit ProjectForm(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			const std::string& projectType
		);
		void Constructor(FD::ProjectWrite,FD::ProjectRead, FD::GuiRead);

		~ProjectForm() noexcept;

	public:
		virtual void call() override;

	private://data
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;

		FD::Text::ProjectForm text{};

	private://data

		const std::string projectType;

		std::string folderPathStr{};
		std::string projectNameStr{};

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			float innerPosHeight = 0.0f;
		}style;

		struct {
			ImVec2 projectFolder{};
			ImVec2 projectName{};
			ImVec2 create{};
		}pos;

	private:
		void title();
		void folderPath();
		void projectName();

		void bottom();

	private:
		void createDirectory();
		_NODISCARD bool createProject();


	};
}