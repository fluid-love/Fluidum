#pragma once

#include "../../Common/common.h"

namespace FS {

	class TextEditor final : public Scene {
	public:
		explicit TextEditor(
			FD::Coding::TabWrite* const tabWrite,
			const FD::Coding::TabRead* const tabRead,
			const FD::GuiRead* const guiRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const std::string& path = {}
		);
		void Constructor(FD::Coding::TabWrite, FD::Coding::TabRead, FD::GuiRead, FD::ProjectWrite, FD::ProjectRead);

		~TextEditor() noexcept;

		FluidumUtils_Class_Delete_CopyMove(TextEditor)

	public:
		virtual void call() override;

	private://data
		FD::Coding::TabWrite* const tabWrite;
		const FD::Coding::TabRead* const tabRead;
		const FD::GuiRead* const guiRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;

		FD::Text::TextEditor text{};


	private://data

		struct Size final {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();
		}size;

		struct Info final {
			FTE::TextEditor* editor = nullptr;
			std::string path{};
		};
		std::vector<Info> info{};

		FTE::TextEditor* editor = nullptr;

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
		void textChange();
		std::string getCurrentEditorPath();

	};
}