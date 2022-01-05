#pragma once

#include "../../Common/common.h"

namespace FS::Project::File {

	class SaveAs final : public Scene {
	public:
		explicit SaveAs(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			const FD::ExitRead* const exitRead,
			FD::WindowWrite* const windowWrite
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::GuiRead,
			FD::ExitRead,
			FD::WindowWrite
		);

		~SaveAs() noexcept;

		FluidumUtils_Class_Delete_CopyMove(SaveAs)

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;
		FD::WindowWrite* const windowWrite;

		FD::Text::ProjectSaveAs text{};

	private:
		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			float innerPosHeight = 0.0f;
		}style;

		struct {
			std::string directoryPath{};
			std::string projectName{};
		}str;

		struct {
			ImVec2 projectFolder{};
			ImVec2 projectName{};
			ImVec2 save{};
		}pos;

		const bool exit;

	private:
		void title();

		void folderPath();
		void projectName();

		void bottom();

		bool save();

		void checkExit();

	};

}