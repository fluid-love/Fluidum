#pragma once

#include "../../Common/common.h"

namespace FS {

	class LeftBar final :public Scene {
	public:
		explicit LeftBar(
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::SceneRead* const sceneRead,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::TabWrite* const tabWrite,
			std::vector<FDR::ImGuiImage>&& images
		);
		void Constructor(
			FD::ProjectRead,
			FD::GuiRead,
			FD::GuiWrite,
			FD::SceneRead,
			FD::Coding::TabRead,
			FD::Coding::TabWrite
		);

		~LeftBar() noexcept;

		FluidumUtils_Class_Delete_CopyMove(LeftBar);

	public:
		virtual void call() override;

	private://using
		using ClassCode = FU::Class::ClassCode;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const guiRead;
		const FD::SceneRead* const sceneRead;
		const FD::Coding::TabRead* const tabRead;
		FD::Coding::TabWrite* const tabWrite;

		FD::Text::LeftBar text{};

	private:
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

		enum class SceneIndex : UIF8 {
			None = 0,
			Coding = 1,
			AnalysisOverview = 3,
			Genome = 4
		};

		struct {
			bool isIconHovered = false;
			bool isSubWindowHovered = false;
			ImVec2 selectWindowPos = ImVec2();
			std::pair<ImVec2, ImVec2> hoveringImageRect{};
			SceneIndex current = SceneIndex::None;

			std::vector<FDR::ImGuiImage> codingImages{};
			std::vector<FDR::ImGuiImage> analysisImages{};
			std::vector<FDR::ImGuiImage> genomeImages{};

		}sub;

		struct {
			bool deleteScene{};
		}flag;

	private:
		void imageGui();
		void hoveredIcon(const Size index);

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
		void subWindow();

		void subWindowCoding();
		void subWindowAnalysis();
		void subWindowGenome();

		void addCodingSubScene(const ClassCode::CodeType code);
		void addAnalysisSubScene(const ClassCode::CodeType code);
		void addGenomeSubScene(const ClassCode::CodeType code);

		void deleteCodingSubScene(const ClassCode::CodeType code);
		void deleteAnalysisSubScene(const ClassCode::CodeType code);
		void deleteGenomeSubScene(const ClassCode::CodeType code);

	private:
		void subWindowHelpSetting();

	private:
		void drawRightBorder();

	private:
		void mini();
		void clickedMiniIcons(const ClassCode::CodeType code, const bool exists);
		void taskManagerScene(const bool exists);

	};

}