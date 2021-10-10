#pragma once

#include "../../Common/common.h"

namespace FS {

	class LeftBar final :public Scene {
	public:
		//時間がかかるのでleftbarのアイコンはあらかじめ準備しておいてもらう
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
		std::array<ClassCode::CodeType, 9> test;

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
		}color;

		enum SceneIndex : uint8_t {
			None = 0,
			Coding = 1
		};

		struct SubWindow final {
			bool isIconHovered = false;
			bool isSubWindowHovered = false;
			ImVec2 selectWindowPos = ImVec2();
			SceneIndex current = SceneIndex::None;

			std::optional<std::array<FDR::ImGuiImage, 1>> codingImages = std::nullopt;
		}sub;



	private:

		void imageGui();
		void addScene(const ClassCode::CodeType code);
		void addCodingScene();
		void addProjectScene();
		void deleteScene(const ClassCode::CodeType code);

		//ImageButtonのHovered時
		void subWindow();

		void subWindowCoding();

		void addCodingScene(const ClassCode::CodeType code);
		void deleteCodingScene(const ClassCode::CodeType code);



		void subWindowHelpSetting();
	};
}