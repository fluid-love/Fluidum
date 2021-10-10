#pragma once

#include "../Common/common.h"

namespace FS {

	class Title final : public Scene {
	public:
		explicit Title(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::SceneRead* const sceneRead,
			FD::GuiWrite* const guiWrite,
			const FD::GuiRead* const guiRead
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::SceneRead,
			FD::GuiWrite,
			FD::GuiRead
		);

		~Title() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Title)

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::SceneRead* const sceneRead;
		FD::GuiWrite* const guiWrite;
		const FD::GuiRead* const guiRead;

		FD::Text::Title text{};

	private:
		std::once_flag once{};
		std::array<FD::Project::HistoryInfo, 50> recentProjectInfos;

		struct {
			ImVec2 open{};
			ImVec2 recentButton{};
		}pos;

	private:
		//constructorはまだFluidumDrawの初期化が終わってないのでループ内で一回だけよぶ
		void writeGuiData();

		//マウスをクリックしたらシーンを切り替え
		void changeScene();

		//タイトル画像の描写
		void drawTitleImage();

		//select recent project
		void selectProject();

		void recentProject();

		void newProject();
		void openProjectButton();
		void openProject(const char* filePath,const ImVec2& pos);
		void documentLink();

	private:

		//1920×1080 title画像
		//画像データ
		//Titleシーンは特別で，コンストラクタ時点でFluidumDrawは初期化されていないのでoptionalで遅延する．
		std::optional<FDR::ImGuiImage> image = std::nullopt;

		//recent
		std::optional<FDR::ImGuiImage> projectIcon = std::nullopt;

		//project windowがhoveredされていれば次に進ませない
		bool isSelectProjectHovered = false;

		//style
		struct {
			const ImVec2 imageHalfSize = ImVec2(960, 540);
			ImVec2 windowPos{};

			ImVec2 selectWindowPos{};
			ImVec2 selectWindowSize{};

		}style;

		//時間がかかるのでこっちで準備しておく
		std::optional<std::vector<FDR::ImGuiImage>> leftBarImages = std::nullopt;

	};
}