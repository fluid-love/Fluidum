/*
MenuBar->新規作成
*/

#pragma once

#include <unordered_map>
#include "../../../Common/common.h"

namespace FS::Bar {

	class NewProject final : public Scene {
	public:
		explicit NewProject(
			const FD::SceneRead* const sceneRead,
			FD::WindowWrite* const windowWrite,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite
		);
		void Constructor(FD::SceneRead, FD::WindowWrite, FD::GuiRead, FD::GuiWrite);

		~NewProject() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::SceneRead* const sceneRead;
		FD::WindowWrite* const windowWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;

		FD::Text::NewProject text{};

	private://data
		const std::vector<FDR::ImGuiImage> images;

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			ImVec2 buttonSize{};
		}style;

		enum class Template : uint8_t {
			Empty,
			Algorithm
		};
		struct ButtonInfo final {
			FDR::ImGuiImage image;
			const char* label;
			const char* title;
			const char* description;
		};
		//雛形　空
		const std::vector<ButtonInfo> emptyTemplates;
		//雛形
		const std::vector<ButtonInfo> algorithmTemplates;

		//filter　すくないのでまだ実装しない
		const std::unordered_map<Template, const ButtonInfo&> templates{};

	private:
		void title();
		void recent();
		void select();
		void filter();
		void buttons();

		void bottom();

		void formScene(const bool isButtonClicked, const char* type);

	private:
		bool button(const FDR::ImGuiImage& image, const char* label, const char* title, const char* description);

	};
}