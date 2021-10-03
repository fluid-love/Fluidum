#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS {

	class TextEditor final : public Scene {
	public:
		explicit TextEditor(
			const FD::GuiRead* const guiRead,
			const std::string& path = {}
		);
		void Constructor(FD::GuiRead);

		~TextEditor() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::GuiRead* const guiRead = nullptr;

		//	Text::Coding text;

		FTE::TextEditor editor{};

	private://data

		struct Size final {
			ImVec2 windowPos = ImVec2();
			ImVec2 windowSize = ImVec2();
		}size;


	private:
		void textEditorMenu();
		void textEditor();
		void textEditorInfo();

	};
}