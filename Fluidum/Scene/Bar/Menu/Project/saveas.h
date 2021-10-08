#pragma once

#include "../../../Common/common.h"

namespace FS::Bar {

	class SaveAs final : public Scene {
	public:
		explicit SaveAs(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead
		);
		void Constructor(FD::ProjectWrite, FD::ProjectRead, FD::GuiRead);

		~SaveAs() noexcept;

	public:
		virtual void call() override;

	private://data
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;

		FD::Text::ProjectSaveAs text{};

	private://data

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			float innerPosHeight = 0.0f;
		}style;

		std::string folderPathStr;
		std::string projectNameStr;

		struct {
			ImVec2 projectFolder{};
			ImVec2 projectName{};
			ImVec2 save{};
		}pos;

	private:
		void title();

		void folderPath();
		void projectName();

		void bottom();

		bool save();
	};
}