#pragma once

#include "../Common/common.h"

namespace FS {

	class Title final : public Scene {
	public:
		explicit Title(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::SceneRead* const sceneRead,
			FD::GuiWrite* const guiWrite,
			const FD::GuiRead* const guiRead,
			const FD::Style::VarRead* const varRead,
			FD::Style::ThemeWrite* const themeWrite
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::SceneRead,
			FD::GuiWrite,
			FD::GuiRead,
			FD::Style::VarRead,
			FD::Style::ThemeWrite
		);

		~Title() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Title);

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::SceneRead* const sceneRead;
		FD::GuiWrite* const guiWrite;
		const FD::GuiRead* const guiRead;
		const FD::Style::VarRead* const varRead;
		FD::Style::ThemeWrite* const themeWrite;

		FD::Text::Title text{};

	private:
		std::array<FD::ProjectRead::HistoryInfo, FD::Project::Limits::HistoryLogMax> recentProjectInfo;

		struct {
			ImVec2 recentWindow{};
			ImVec2 open{};
			ImVec2 document{};
			ImVec2 recentButton{};
		}pos;

		//1920Å~1080 
		/*
		The Title scene is special.
		FluidumDraw is not initialized at the constructor,
		so it is delayed by std::optional.
		*/
		std::optional<FDR::ImGuiImage> image = std::nullopt;

		//recent
		std::optional<FDR::ImGuiImage> projectIcon = std::nullopt;

		struct {
			std::once_flag once{};
			bool projectHistoryError = false;
			bool isSelectProjectHovered = false;
			bool recentProjectPopup = false;
			bool recentProjectButtonPopup = false;
			bool recentProjectButtonHovered = false;
		}flag;

		struct {
			const FD::ProjectRead::HistoryInfo* info = nullptr;
		}buttonPopup;

		struct {
			const ImVec2 imageHalfSize = ImVec2(960, 540);
			ImVec2 windowPos{};
			ImVec2 selectWindowPos{};
			ImVec2 selectWindowSize{};
		}style;

		//Since it takes time, load the image for the next scene.
		std::optional<std::vector<FDR::ImGuiImage>> leftBarImages = std::nullopt;

	private:
		//no-throw
		[[nodiscard]] std::array<FD::ProjectRead::HistoryInfo, FD::Project::Limits::HistoryLogMax> getProjectHistory() noexcept;

	private:
		//The constructor hasn't finished initializing FluidumDraw yet. Therefore, call it only once in the loop.
		void writeGuiData();

		//Change scenes with a click of the mouse.
		void changeScene();

		//title image
		void drawTitleImage();

	private:
		//select recent project
		void selectProject();

		void recentProject();
		
		void recentProjectPopup();
		void recentProjectButtonPopup();
		void recentPopupItem_clearAll();

	private:
		void newProject();

	public:
		void openProjectButton();
		void openProject(const char* filePath, const ImVec2& pos);

	private:
		void documentLink();
		[[nodiscard]] bool checkIsShellAvailable() const;

	};

}