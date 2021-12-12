#pragma once

#include "../../Common/common.h"

namespace FS {

	class TitleBar final : public Scene {
	public:
		explicit TitleBar(
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::WindowRead* const windowRead,
			FD::WindowWrite* const windowWrite,
			const FD::SceneRead* const sceneRead,
			const FD::ProjectRead* const projectRead,
			FD::ExitWrite* const exitWrite,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::TabWrite* const tabWrite
		);
		void Constructor(
			FD::GuiRead,
			FD::GuiWrite,
			FD::WindowRead,
			FD::WindowWrite,
			FD::SceneRead,
			FD::ProjectRead,
			FD::ExitWrite,
			FD::Coding::TabRead,
			FD::Coding::TabWrite
		);

		~TitleBar() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;
		const FD::WindowRead* const windowRead;
		FD::WindowWrite* const windowWrite;
		const FD::SceneRead* const sceneRead;
		const FD::ProjectRead* const projectRead;
		FD::ExitWrite* const exitWrite;
		const FD::Coding::TabRead* const tabRead;
		FD::Coding::TabWrite* const tabWrite;

	private://data

		const FD::Text::TitleBar text{};

		const FDR::ImGuiImage iconImage = FDR::createImGuiImage(Resource::TitleBarIconFilePath);

		struct {
			ImVec2 iconWindowPos{};
			ImVec2 iconWindowSize{};

			ImVec2 windowPos{};
			ImVec2 windowSize{};

			ImVec2 projectNameWindowPos{};
			ImVec2 projectNameWindowSize{};


			ImVec2 iconSize{};

			ImVec2 buttonSize{};
		}style;

	private:
		void icon();
		void bar();
		void project();

	private:
		void exit();

	};
}