#pragma once

#include "../../Common/common.h"

namespace FS {

	class TextEditor final : public Scene {
	public:
		explicit TextEditor(
			const FD::Style::VarRead* const varRead,
			FD::Coding::TabWrite* const tabWrite,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::DisplayWrite* const displayWrite,
			const FD::Coding::DisplayRead* const displayRead,
			const FD::GuiRead* const guiRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::ProjectFilesRead* const projectFilesRead,
			FD::ToolBarWrite* const toolBarWrite
		);
		void Constructor(
			FD::Style::VarRead,
			FD::Coding::TabWrite,
			FD::Coding::TabRead,
			FD::Coding::DisplayWrite,
			FD::Coding::DisplayRead,
			FD::GuiRead,
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::ProjectFilesRead,
			FD::ToolBarWrite
		);

		~TextEditor() noexcept;

		FluidumUtils_Class_Delete_CopyMove(TextEditor)

	public:
		virtual void call() override;

	private://data
		const FD::Style::VarRead* const varRead;
		FD::Coding::TabWrite* const tabWrite;
		const FD::Coding::TabRead* const tabRead;
		FD::Coding::DisplayWrite* const displayWrite;
		const FD::Coding::DisplayRead* const displayRead;
		const FD::GuiRead* const guiRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		FD::ToolBarWrite* const toolBarWrite;

		FD::Text::TextEditor text{};

	private://data

		struct Size final {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();
		}size;

		struct Info final {
			FTE::TextEditor* editor = nullptr;
			std::string path{};
			FD::Project::File::SupportedFileType language{};
		};
		std::vector<Info> info{};

		Info* current = nullptr;

	private:
		void toolBar();

	private:
		void windowEmpty();
		void setInfo();

	private:
		void textEditorMenu();
		void fileMenu();
		void editMenu();
		void themeMenu();

		void textEditor();
		void breakPoint();
		void textEditorInfo();

	private:
		void saveText();
		void saveAs();

		void update();
		void textChange();
		std::string getCurrentEditorPath();

	private:
		std::chrono::system_clock::time_point checkSyntaxTimePoint{};
		void checkSyntaxError();
		void checkLua();
		void checkPython();
		void checkAngelScript();
		lua_State* luaState = nullptr;

	};

}