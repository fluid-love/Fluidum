#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS::Coding {

	class New final : public Scene {
	public:
		explicit New(
			FD::Coding::TabWrite* const tabWrite,
			const FD::ProjectRead* const projectRead,
			FD::ProjectWrite* const projectWrite,
			const FD::GuiRead* const guiRead
		);
		void Constructor(FD::Coding::TabWrite, FD::ProjectRead, FD::ProjectWrite, FD::GuiRead);

		~New() noexcept;

		FluidumUtils_Class_Delete_CopyMove(New)

	public:
		virtual void call() override;

	private://data
		FD::Coding::TabWrite* const tabWrite;
		const FD::ProjectRead* const projectRead;
		FD::ProjectWrite* const projectWrite;


		FD::Text::CodingNew text{};

	private:
		const std::string projectType;

		std::string folderPath{};
		std::string fileName{};

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

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
	};
}