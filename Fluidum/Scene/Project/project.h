#pragma once

#include "../Common/common.h"
#include "../File/new.h"

namespace FS::Project {

	class Explorer final : public Scene {
	public:
		explicit Explorer(
			FD::ImGuiWindowWrite* const imguiWindowWrite,
			const FD::Style::ColorRead* const colorRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::Project::PropertyWrite* const propertyWrite,
			const FD::Project::PropertyRead* const propertyRead,
			FD::Project::PropertyLuaWrite* const propertyLuaWrite,
			const FD::Project::PropertyLuaRead* const propertyLuaRead,
			FD::ProjectFilesWrite_Lock* const projectFilesWrite,
			const FD::ProjectFilesRead_Lock* const projectFilesRead,
			FD::UserFilesWrite_Lock* const userFilesWrite,
			const FD::UserFilesRead_Lock* const userFilesRead,
			const FD::SceneRead* const sceneRead,
			FD::Coding::TabWrite* const tabWrite,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::DisplayWrite* const displayWrite,
			const FD::Coding::DisplayRead* const displayRead,
			FD::ToolBarWrite* const toolBarWrite,
			const FD::Style::VarRead* const varRead
		);
		void Constructor(
			FD::ImGuiWindowWrite,
			FD::Style::ColorRead,
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::Project::PropertyWrite,
			FD::Project::PropertyRead,
			FD::Project::PropertyLuaWrite,
			FD::Project::PropertyLuaRead,
			FD::ProjectFilesWrite_Lock,
			FD::ProjectFilesRead_Lock,
			FD::UserFilesWrite_Lock,
			FD::UserFilesRead_Lock,
			FD::SceneRead,
			FD::Coding::TabWrite,
			FD::Coding::TabRead,
			FD::Coding::DisplayWrite,
			FD::Coding::DisplayRead,
			FD::ToolBarWrite,
			FD::Style::VarRead
		);

		~Explorer() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Explorer);

	public:
		virtual void call() override;

	private:
		FD::ImGuiWindowWrite* const imguiWindowWrite;
		const FD::Style::ColorRead* const colorRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		FD::Project::PropertyWrite* const propertyWrite;
		const FD::Project::PropertyRead* const propertyRead;
		FD::Project::PropertyLuaWrite* const propertyLuaWrite;
		const FD::Project::PropertyLuaRead* const propertyLuaRead;
		FD::ProjectFilesWrite_Lock* const projectFilesWrite;
		const FD::ProjectFilesRead_Lock* const projectFilesRead;
		FD::UserFilesWrite_Lock* const userFilesWrite;
		const FD::UserFilesRead_Lock* const userFilesRead;
		const FD::SceneRead* const sceneRead;
		FD::Coding::TabWrite* const tabWrite;
		const FD::Coding::TabRead* const tabRead;
		FD::Coding::DisplayWrite* const displayWrite;
		const FD::Coding::DisplayRead* const displayRead;
		FD::ToolBarWrite* const toolBarWrite;
		const FD::Style::VarRead* const varRead;

		FD::Text::Project text{};

	private:
		struct {
			float topBarHeight{};
		}style;


		struct {
			ImVec2 sync{};
			ImVec2 selectedTree{};
		}pos;

		enum class TabType : UIF8 {
			Fluidum,
			Project,
			User
		};

		struct {
			TabType tab = TabType::Fluidum;
			FD::Project::FileList::FileInfo* projectFiles = nullptr;
			FD::Project::FileList::FileInfo* userFiles = nullptr;

			inline FD::Project::FileList::FileInfo* current() noexcept {
				if (tab == TabType::Project)
					return this->projectFiles;
				else if (tab == TabType::User)
					return this->userFiles;
				else
					return nullptr;
			}
		}select;

		enum class PopupType : UIF8 {
			Top,
			Dir,
			Supported,
			Unsupported,
		};

		struct {
			PopupType type{};

			bool top = false;
			static constexpr const char* Top = "TopPopup";

			bool dir = false;
			static constexpr const char* Dir = "DirPopup";

			bool supported = false;
			static constexpr const char* Supported = "SupPopup";

			bool unsupported = false;
			static constexpr const char* Unsupported = "UnsupPopup";

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
			std::shared_ptr<::FS::File::Add::SharedInfo> info{};
		}add;

		struct {
			ImCounter<ImAnimeTime> sync{};
			ImCounter<ImAnimeTime> collapseAll{};
			ImCounter<ImAnimeTime> displayCode{};
		}anime;

		struct {
			FD::Project::File::SupportedFileType fileType{};

		}supportedPopupInfo;

		bool windowFlag = true;

	private://misc
		void closeWindow();
		void toolBar();
		void setImGuiWindow();

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
		void releaseFile();
		void displayCode();
		void flipOpen();
		void collapseAll();
		void rename();
		void setMainFile();

	private:
		void openPopup();

		void topPopup();
		void directoryPopup();
		void supportedPopup();
		void unsupportedPopup();

		void popup_add();

		void changeNamePopup();
		void tryChangeName();
		[[nodiscard]] std::pair<bool, std::string> checkChangeName();

	private:
		void syncProjectFiles();
		I32 popupSpacing();

	};

}