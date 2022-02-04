#pragma once

#include "../../Common/common.h"

namespace FS {

	class MenuBar final : public Scene {
	public:
		explicit MenuBar(
			const FD::Style::ColorRead* const colorRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::WindowWrite* const windowWrite,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			FD::Coding::TabWrite* const tabWrite,
			const FD::Coding::TabRead* const tabRead,
			const FD::CalcRead* const calcRead,
			FD::CalcWrite* const calcWrite,
			const FD::SceneRead* const sceneRead
		);
		void Constructor(
			FD::Style::ColorRead,
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::WindowWrite,
			FD::GuiRead,
			FD::GuiWrite,
			FD::Coding::TabWrite,
			FD::Coding::TabRead,
			FD::CalcRead,
			FD::CalcWrite,
			FD::SceneRead
		);

		~MenuBar() noexcept;

		FluidumUtils_Class_Delete_CopyMove(MenuBar);

	public:
		virtual void call() override;

	private:
		const FD::Style::ColorRead* const colorRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		FD::WindowWrite* const windowWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;
		FD::Coding::TabWrite* const tabWrite;
		const FD::Coding::TabRead* const tabRead;
		const FD::CalcRead* const calcRead;
		FD::CalcWrite* const calcWrite;
		const FD::SceneRead* const sceneRead;

		FD::Text::MenuBar text{};

	private:
		struct {
			ImVec2 offset{};
			ImVec2 itemSpacing{};
		}style;

		struct {
			float file = 0.0f;
			float edit = 0.0f;
			float calc = 0.0f;
			float view = 0.0f;
			float project = 0.0f;
			float extension = 0.0f;
			float window = 0.0f;
			float help = 0.0f;
		}menuBarItemWidth;

		struct {
			ImVec2 open{};
		}pos;

		struct {
		public:
			bool coding{};
			bool tab{};
			bool debug{};

			bool flu{};

			bool analysis{};
			bool plot{};
			bool function{};

			bool genome{};

			bool animation{};

			bool project{};

			bool console{};
		}view;

	private:
		//file
		void fileGui();
		void itemCreateNewProject();
		void itemOpen();
		void itemSave();
		void itemSaveAs();
		void itemExit();

		//edit
		void editGui();

	private://calc
		void calcGui();
		void itemRunDebugMode();
		void itemRunNormalMode();

	private://view
		void viewGui();
		void updateView();
		void itemCoding();
		void itemFlu();
		void itemAnalysis();
		void itemGenome();
		void itemAnimation();
		void itemProject();
		void itemConsole();

	private://project
		void projectGui();
		void project_property();

	private://extention
		void extensionGui();

	private://window
		void windowGui();
		void itemLayoutTemplates();
		[[nodiscard]] bool layoutConfirm();
		void setLayoutEmpty();
		void setLayoutCoding();

	private://help
		void helpGui();

	private:
		void pushItemSpacing();

	};

}