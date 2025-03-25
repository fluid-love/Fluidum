#pragma once

#include "../../Common/common.h"

namespace FS::Project::File {

	class Form final : public Scene {
	public:
		struct SharedInfo final {
		public://arg
			const FD::History::Project::Type type;
			const std::string name;
		public://return
			bool create = false;
		};

	public:
		explicit Form(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			std::shared_ptr<SharedInfo>& info
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::GuiRead
		);

		~Form() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Form);

	public:
		virtual void call() override;

	private://data
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;

		FD::Text::ProjectForm text{};

	private:
		std::shared_ptr<SharedInfo> info;

		struct {
			std::string directoryPath{};
			std::string name{};
		}str;

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			float innerPosHeight = 0.0f;
		}style;

		struct {
			ImVec2 projectDirectory{};
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
		[[nodiscard]] bool createProject();

	};

}