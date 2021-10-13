#pragma once

#include "../Common/common.h"

namespace FS {

	class Project final : public Scene {
	public:
		explicit Project(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::SceneRead* const sceneRead,
			FD::Coding::TabWrite* const tabWrite
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::SceneRead,
			FD::Coding::TabWrite
		);

		~Project() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Project)

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::SceneRead* const sceneRead;
		FD::Coding::TabWrite* const tabWrite;

		FD::Text::Project text{};

	private:
		struct {
			float topBarHeight{};
		}style;


		struct {
			ImVec2 sync{};
		}pos;

		struct {
			int8_t SLLIndex = -1;
			bool codeOpen = false;
		}select;

	public:
		struct Proj final {
			std::string projectName{};

			struct Info final {
				std::vector<Info> dir_internal{};
				bool isDirectory = false;
				bool open = false;
				std::string path{};
				std::string name{};
			};

			//is directory,open,path str
			std::vector<Info> listOfPaths{};
		}project;

	private:
		void topBar();

		void body1();
		void body2();

		void displayCode(const Proj::Info& info);

		void syncListOfFiles();

	};

}