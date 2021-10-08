#pragma once

#include "../../Common/common.h"

namespace FS {

	class MenuBar final : public Scene {
	public:
		explicit MenuBar(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::WindowWrite* const windowWrite,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::WindowWrite, 
			FD::GuiRead, 
			FD::GuiWrite
		);

		~MenuBar() noexcept;

	public:
		virtual void call() override;

	private://data
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		FD::WindowWrite* const windowWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;

		FD::Text::MenuBar text{};

	private://data

		struct {
			ImVec2 offset{};
		}style;


	private:

		//file
		void fileGui();
		void itemCreateNewProject();
		void itemOpen();
		void itemSave();
		void itemSaveAs();
		void saveAs(std::string path);
		void itemTerminate();

		//edit
		void editGui();

		//calc
		void calc();

		//view
		void viewGui();

		//help
		void helpGui();
	};
}