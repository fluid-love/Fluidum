#pragma once

#include "projectform.h"
#include "../../../Common/common.h"

namespace FS::Bar {

	class NewProject final : public Scene {
	public:
		explicit NewProject(
			const FD::SceneRead* const sceneRead,
			FD::WindowWrite* const windowWrite,
			const FD::GuiRead* const guiRead,
			FD::GuiWrite* const guiWrite,
			const FD::Log::ProjectRead* const projectLogRead,
			FD::Log::ProjectWrite* const projectLogWrite
		);
		void Constructor(
			FD::SceneRead,
			FD::WindowWrite,
			FD::GuiRead,
			FD::GuiWrite,
			FD::Log::ProjectRead,
			FD::Log::ProjectWrite
		);

		~NewProject() noexcept;

	public:
		virtual void call() override;

	private:
		const FD::SceneRead* const sceneRead;
		FD::WindowWrite* const windowWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;
		FD::Log::ProjectWrite* const projectLogWrite;

		FD::Text::NewProject text{};

	private:
		std::shared_ptr<ProjectForm::Info> formInfo{};
		void checkForm();

	private:
		const std::vector<FDR::ImGuiImage> images;

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

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
			FD::Log::Project::Type type;
			FDR::ImGuiImage image;
			const char* label;
			const std::string& title;
			const char* description;

			bool hide = false;
		};

		//empty
		std::vector<ButtonInfo> emptyTemplates;

		//algorithm
		std::vector<ButtonInfo> algorithmTemplates;

		//recent
		const std::vector<ButtonInfo> recentTemplates;

		struct {
			std::string str{};
			std::string temp{};
		}searchStr;

		struct {
			uint16_t index{};
			bool flag = false;
		}recentPopupInfo;

	private:
		void title();
		void recent();
		void select();
		void filter();
		void buttons();

		void bottom();

		void formScene(const bool isButtonClicked, const ButtonInfo& button);

	private:
		//return [left, right]
		std::pair<bool, bool> button(
			const FDR::ImGuiImage& image,
			const char* label,
			const char* title,
			const char* description,
			const std::optional<float> width = std::nullopt
		);

	private:
		void search();

	private:
		void recentPopup();
		void recent_clear();
		void recent_erase();

	private:
		std::vector<ButtonInfo> initRecentTempates(const std::vector<FD::Log::Project::Type>& types);
	};
}