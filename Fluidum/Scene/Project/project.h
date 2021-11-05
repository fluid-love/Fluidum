#pragma once

#include "../Common/common.h"

namespace FS {

	class Project final : public Scene {
	public:
		explicit Project(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::FluidumFilesWrite* const fluidumFilesWrite,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			FD::ProjectFilesWrite* const projectFilesWrite,
			const FD::ProjectFilesRead* const projectFilesRead,
			FD::UserFilesWrite* const userFilesWrite,
			const FD::UserFilesRead* const userFilesRead,
			const FD::SceneRead* const sceneRead,
			FD::Coding::TabWrite* const tabWrite
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::FluidumFilesWrite,
			FD::FluidumFilesRead,
			FD::ProjectFilesWrite,
			FD::ProjectFilesRead,
			FD::UserFilesWrite,
			FD::UserFilesRead,
			FD::SceneRead,
			FD::Coding::TabWrite
		);

		~Project() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Project)

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		FD::FluidumFilesWrite* const fluidumFilesWrite;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		const FD::ProjectRead* const projectRead;
		FD::ProjectFilesWrite* const projectFilesWrite;
		const FD::ProjectFilesRead* const projectFilesRead;
		FD::UserFilesWrite* const userFilesWrite;
		const FD::UserFilesRead* const userFilesRead;
		const FD::SceneRead* const sceneRead;
		FD::Coding::TabWrite* const tabWrite;

		FD::Text::Project text{};

	private:
		struct {
			float topBarHeight{};
		}style;


		struct {
			ImVec2 sync{};
			ImVec2 selectedTree{};
		}pos;

		enum class TabType : uint8_t {
			Fluidum,
			Project,
			User
		};

		struct {
			TabType tab = TabType::Fluidum;
			FD::Project::List::FileInfo* userFiles = nullptr;
		}select;

		struct {
			bool top = false;
			static constexpr const char* Top = "TopPopup";

			bool dir = false;
			static constexpr const char* Dir = "DirPopup";

			bool code = false;
			static constexpr const char* Code = "CodePopup";

			static constexpr const char* ChangeName = "ChangeNamePopup";

		}popup;

		struct {
			bool popup = false;
			bool once = false;
			std::string name{};
			bool folder = false;
			ImVec2 pos{};
			ImVec2 size{};
		}changeName;

	public:
		struct Proj final {
			std::string projectName{};

			struct Info final {
				std::vector<Info> dir_internal{};
				bool isDirectory = false;
				bool open = false;
				std::string path{};
				std::string name{};

				std::size_t index;
			};

			//is directory,open,path str
			std::vector<Info> listOfPaths{};
		}project;

		std::size_t selectedIndex = 0;

	private:
		void topBar();

	private:
		void tab();

	private:
		void fluidumFiles();
		void mainCodeFile();
		void standardFluidumLibrary();

	private:
		void projectFiles();

	private:
		std::pair<ImVec2, ImVec2> userFilesTree(std::vector<FD::Project::List::FileInfo>* node, FD::Project::List::FileInfo* info);
		void userFiles();
		void addVirtualFolder();
		void eraseVirtualFolder();

	private:
		void addFile();

	private:
		void openPopup();

		void topPopup();
		void directoryPopup();
		void codePopup();

		void changeNamePopup();
		void tryChangeName();
		_NODISCARD bool checkChangeName();

	private:

		void displayCode(const Proj::Info& info);

		void syncListOfFiles();

	};

}