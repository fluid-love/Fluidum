#pragma once

#include "../Common/common.h"
#include "Add/new.h"

namespace FS::Project {

	class Explorer final : public Scene {
	public:
		explicit Explorer(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::LuaFilesWrite_Lock* const luaFilesWrite,
			FD::FluidumFilesWrite* const fluidumFilesWrite,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			FD::ProjectFilesWrite_Lock* const projectFilesWrite,
			const FD::ProjectFilesRead* const projectFilesRead,
			FD::UserFilesWrite_Lock* const userFilesWrite,
			const FD::UserFilesRead* const userFilesRead,
			const FD::SceneRead* const sceneRead,
			FD::Coding::TabWrite* const tabWrite
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::LuaFilesWrite_Lock,
			FD::FluidumFilesWrite,
			FD::FluidumFilesRead,
			FD::ProjectFilesWrite_Lock,
			FD::ProjectFilesRead,
			FD::UserFilesWrite_Lock,
			FD::UserFilesRead,
			FD::SceneRead,
			FD::Coding::TabWrite
		);

		~Explorer() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Explorer)

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		FD::LuaFilesWrite_Lock* const luaFilesWrite;
		FD::FluidumFilesWrite* const fluidumFilesWrite;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		const FD::ProjectRead* const projectRead;
		FD::ProjectFilesWrite_Lock* const projectFilesWrite;
		const FD::ProjectFilesRead* const projectFilesRead;
		FD::UserFilesWrite_Lock* const userFilesWrite;
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
			FD::Project::FileList::FileInfo* fluidumFiles = nullptr;
			FD::Project::FileList::FileInfo* projectFiles = nullptr;
			FD::Project::FileList::FileInfo* userFiles = nullptr;

			inline FD::Project::FileList::FileInfo* current() noexcept {
				if (tab == TabType::Project)
					return this->projectFiles;
				else if (tab == TabType::User)
					return this->userFiles;
				else
					return this->fluidumFiles;
			}
		}select;

		enum class PopupType : uint8_t {
			Top,
			Dir,
			Supported
		};

		struct {
			PopupType type{};

			bool top = false;
			static constexpr const char* Top = "TopPopup";

			bool dir = false;
			static constexpr const char* Dir = "DirPopup";

			bool supported = false;
			static constexpr const char* Supported = "SupportedPopup";

			static constexpr const char* ChangeName = "ChangeNamePopup";

		}popup;

		struct {
			bool popup = false;
			bool once = false;
			std::string tempName{};
			std::string name{};
			bool folder = false;
			ImVec2 pos{};
			ImVec2 size{};
		}changeName;

		struct {
			bool popup = false;
			std::shared_ptr<Add::SharedInfo> info{};
		}add;

		struct {
			ImCounter<ImAnimeTime> sync{};
			ImCounter<ImAnimeTime> collapseAll{};
			ImCounter<ImAnimeTime> displayCode{};
		}anime;

		bool windowFlag = false;
	private:
		void closeWindow();

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
		std::pair<ImVec2, ImVec2> projectFilesTree(std::vector<FD::Project::FileList::FileInfo>* node, FD::Project::FileList::FileInfo* info);
		void projectFiles();

	private:
		std::pair<ImVec2, ImVec2> userFilesTree(std::vector<FD::Project::FileList::FileInfo>* node, FD::Project::FileList::FileInfo* info);
		void userFiles();

	private:
		void addDirectory();
		void addFile();
		void addSelect();
		void addFileQuick();
		void catchAdd();
		void removeDirectory();
		void removeFile();
		void displayCode();
		void flipOpen();
		void collapseAll();

	private:
		void openPopup();

		void topPopup();
		void directoryPopup();
		void supportedPopup();

		void popup_add();

		void changeNamePopup();
		void tryChangeName();
		[[nodiscard]] bool checkChangeName();

	private:
		void syncProjectFiles();
		int32_t popupSpacing();
	};

}