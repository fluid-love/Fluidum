#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS {

	class TextEditor final : public Scene {
	public:
		explicit TextEditor(
			const FD::Coding::TabRead* const tabRead,
			const FD::GuiRead* const guiRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const std::string& path = {}
		);
		void Constructor(FD::Coding::TabRead, FD::GuiRead, FD::ProjectWrite, FD::ProjectRead);

		~TextEditor() noexcept;

		FluidumUtils_Class_Delete_CopyMove(TextEditor)

	public:
		virtual void call() override;

	private://data
		const FD::Coding::TabRead* const tabRead;
		const FD::GuiRead* const guiRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;

		FD::Text::TextEditor text{};

		FTE::TextEditor editor{};

	private://data

		struct Size final {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();
		}size;

		struct {
			std::string currentPath{};
		}info;

	private:
		void textEditorMenu();
		void fileMenu();
		void editMenu();
		void themeMenu();

		void textEditor();
		void textEditorInfo();

	private:
		void saveText();

		void update();
	};
}