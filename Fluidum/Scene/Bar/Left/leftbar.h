#pragma once

#include "../../Common/common.h"

namespace FS {

	class LeftBar final :public Scene {
	public:
		//éûä‘Ç™Ç©Ç©ÇÈÇÃÇ≈leftbarÇÃÉAÉCÉRÉìÇÕÇ†ÇÁÇ©Ç∂ÇﬂèÄîıÇµÇƒÇ®Ç¢ÇƒÇ‡ÇÁÇ§
		explicit LeftBar(
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::SceneRead* const sceneRead,
			std::vector<FDR::ImGuiImage>&& images
		);
		void Constructor(FD::ProjectRead, FD::GuiRead, FD::GuiWrite, FD::SceneRead);

		~LeftBar() noexcept;

	public:
		virtual void call() override;

	private://using
		using ClassCode = FU::Class::ClassCode;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::GuiRead* const  guiRead;
		const FD::SceneRead* const  sceneRead;



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

			std::optional<std::array<FDR::ImGuiImage, 1>> codingImages = std::nullopt;
			std::vector<FDR::ImGuiImage> analysisImages{};

		}sub;

		


	private:

		void imageGui();
		void hoveredIcon(const std::size_t index);

		void addScene(const ClassCode::CodeType code);
		void addCodingScene();
		void addAnalysisScene();
		void addProjectScene();

		void deleteScene(const ClassCode::CodeType code);

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