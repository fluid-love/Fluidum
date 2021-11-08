#pragma once

#include "../Common/common.h"
#include "../Coding/Select/new.h"

namespace FS {

	class Project final : public Scene {
	public:
		explicit Project(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::LuaFilesWrite* const luaFilesWrite,
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
			FD::LuaFilesWrite,
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
		FD::LuaFilesWrite* const luaFilesWrite;
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
			FD::Project::List::FileInfo* fluidumFiles = nullptr;
			FD::Project::List::FileInfo* projectFiles = nullptr;
			FD::Project::List::FileInfo* userFiles = nullptr;

			inline FD::Project::List::FileInfo* current() noexcept {
				if (tab == TabType::Project)
					return this->projectFiles;
				else if (tab == TabType::User)
					return this->userFiles;
				else
					return this->fluidumFiles;
			}
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

		struct {
			bool popup = false;
			std::shared_ptr<Coding::New::SharedInfo> info{};
		}newFile;

	private:
		void topBar();
		void showCodeButton();

	private:
		void tab();

	private:
		void fluidumFiles();
		void mainCodeFile();
		void standardFluidumLibrary();

	private:
		std::pair<ImVec2, ImVec2> projectFilesTree(std::vector<FD::Project::List::FileInfo>* node, FD::Project::List::FileInfo* info);
		void projectFiles();
		void createDirectory();

	private:
		std::pair<ImVec2, ImVec2> userFilesTree(std::vector<FD::Project::List::FileInfo>* node, FD::Project::List::FileInfo* info);
		void userFiles();
		void addVirtualFolder();
		void eraseVirtualFolder();
		void eraseUserFile();
		void showCode();
		void flipOpen();

	private:
		void addFile();
		void catchAddFile();

	private:
		void openPopup();

		void topPopup();
		void directoryPopup();
		void codePopup();

		void changeNamePopup();
		void tryChangeName();
		_NODISCARD bool checkChangeName();

	private:
		void syncProjectFiles();
		void collapseAll();
	};

}