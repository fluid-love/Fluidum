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
			FD::ToolBarWrite* const toolBarWrite,
			const FD::Style::ColorRead* const colorRead
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
			FD::ToolBarWrite,
			FD::Style::ColorRead
		);

		~TextEditor() noexcept;

		FluidumUtils_Class_Delete_CopyMove(TextEditor);

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
		const FD::Style::ColorRead* const colorRead;

		FD::Text::TextEditor text{};

	private:
		struct {
			float infoWindowHeight{};
		}style;

		struct {
			std::string input{};
		}zoom;

		struct {
			ImCounter<ImAnimeTime> tool_save{};
			ImCounter<ImAnimeTime> tool_undo{};
			ImCounter<ImAnimeTime> tool_redo{};

		}anime;

		struct {

		}pos;

		struct Info final {
			FTE::TextEditor* editor = nullptr;
			FD::Coding::DisplayInfo info{};
			FD::Project::File::SupportedFileType language{};
		};
		std::vector<Info> info{};

		Info* current = nullptr;
		Info* selected = nullptr;

	private:
		void toolBar();
		void tool_separator();

	private:
		void isWindowFocused();

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

	private://info
		void textEditorInfo();
		void editorInfo_zoom();

	private:
		void saveText(const Info* const  info);
		void saveAs(const Info* const  info);

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

	private://shortcut
		void shortcut();
		void shortcut_zoom();

	};

}