#pragma once

#include "../../Common/common.h"

namespace FS {

	class LeftBar final :public Scene {
	public:
		//éûä‘Ç™Ç©Ç©ÇÈÇÃÇ≈leftbarÇÃÉAÉCÉRÉìÇÕÇ†ÇÁÇ©Ç∂ÇﬂèÄîıÇµÇƒÇ®Ç¢ÇƒÇ‡ÇÁÇ§
		explicit LeftBar(
			const FD::ProjectRead* const projectRead,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::SceneRead* const sceneRead,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::TabWrite* const tabWrite,
			std::vector<FDR::ImGuiImage>&& images
		);
		void Constructor(
			FD::ProjectRead,
			FD::FluidumFilesRead,
			FD::GuiRead,
			FD::GuiWrite,
			FD::SceneRead,
			FD::Coding::TabRead,
			FD::Coding::TabWrite
		);

		~LeftBar() noexcept;

	public:
		virtual void call() override;

	private://using
		using ClassCode = FU::Class::ClassCode;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		const FD::GuiRead* const  guiRead;
		const FD::SceneRead* const  sceneRead;
		const FD::Coding::TabRead* const tabRead;
		FD::Coding::TabWrite* const tabWrite;

		FD::Text::LeftBar text{};

	private://data

		const std::vector<FDR::ImGuiImage> images;

		struct {
			float width = 0.0f;
			float height = 0.0f;

			ImVec2 windowPos{};
			ImVec2 windowSize{};

			ImVec2 imageSize{};
		}style;

		struct Color final {
			ImVec4 dummy = { 0.0f, 0.0f, 0.0f, 0.0f };
			ImVec4 main = { 0.02f, 0.02f, 0.5f, 0.9f };
			ImVec4 sub = { 0.2f, 0.02f, 0.02f, 0.9f };
			ImVec4 analysis = { 0.2f, 0.2f, 0.02f, 0.9f };
		}color;

		enum class SceneIndex : uint8_t {
			None = 0,
			Coding = 1,
			AnalysisOverview = 3,
		};

		struct SubWindow final {
			bool isIconHovered = false;
			bool isSubWindowHovered = false;
			ImVec2 selectWindowPos = ImVec2();
			SceneIndex current = SceneIndex::None;

			std::vector<FDR::ImGuiImage> codingImages{};
			std::vector<FDR::ImGuiImage> analysisImages{};

		}sub;

		


	private:

		void imageGui();
		void hoveredIcon(const std::size_t index);

		void addScene(const ClassCode::CodeType code);
		void addCodingScene();
		void addFluScene();
		void addAnalysisScene();
		void addProjectScene();
		void addAnimationScene();
		void addGenomeScene();
		void addConsoleScene();

	private:
		void deleteScene(const ClassCode::CodeType code);
		void deleteCodingScene();

	private:
		//ImageButtonÇÃHoveredéû
		void subWindow();

		void subWindowCoding();
		void subWindowAnalysis();

		void addCodingSubScene(const ClassCode::CodeType code);
		void addAnalysisSubScene(const ClassCode::CodeType code);

		void deleteCodingSubScene(const ClassCode::CodeType code);
		void deleteAnalysisSubScene(const ClassCode::CodeType code);



		void subWindowHelpSetting();
	};
}