#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS {

	class CodingSelect final : public Scene {
	public:
		explicit CodingSelect(
			FD::Coding::TabWrite* const tabWrite,
			const FD::ProjectRead* const projectRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectFilesRead* const projectFilesRead,
			FD::ProjectFilesWrite* const projectFilesWrite,
			const FD::GuiRead* const guiRead,
			const FD::SceneRead* const sceneRead
		);
		void Constructor(
			FD::Coding::TabWrite,
			FD::ProjectRead,
			FD::ProjectWrite,
			FD::ProjectFilesRead,
			FD::ProjectFilesWrite,
			FD::GuiRead,
			FD::SceneRead
		);

		~CodingSelect() noexcept;

		FluidumUtils_Class_Delete_CopyMove(CodingSelect)

	public:
		virtual void call() override;

	private://data
		FD::Coding::TabWrite* const tabWrite;
		const FD::ProjectRead* const projectRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectFilesRead* const projectFilesRead;
		FD::ProjectFilesWrite* const projectFilesWrite;
		const FD::SceneRead* const sceneRead;

		FD::Text::CodingSelect text{};

	private:

		const FDR::ImGuiImage newImage;
		const FDR::ImGuiImage openImage;

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

		struct {
			ImVec2 windowSize{};
			ImVec2 windowPos{};

			bool isOpenWindowHovered = false;
			bool isNewFileWindowHovered = false;

		}style;

		struct {
			ImVec2 folderPath{};
			ImVec2 fileName{};
			ImVec2 create{};
		}pos;

		enum class Template : uint8_t {
			None,
			Empty_Lua,
			Empty_Py,
			Empty_AS
		};

		Template selectType = Template::None;

		FTE::TextEditor textEditor{};
		bool isFileSelected = false;//color用


		struct {
			std::string folderPath{};
			std::string fileName{};
			std::string extension{};
			std::string fullPath{};
		}quickInfo;


	private:
		//新規作成
		void create();


		//最近つかったもの
		void quick();
		void selectTemplate(const ImVec2& size);

		//開く
		void right();
		void open(const ImVec2& size);
		void openDialog();
		void newFile(const ImVec2& size);

		//右下(クイックにおけるパスとファイル名入力)
		void bottomRight();

		//下
		void bottom();

		void setEmptyFile();

		//クイックで作る場合はプロジェクトフォルダのCodeフォルダに入れる
		void createNewFileQuick();
		_NODISCARD bool checkQuickInfo();

		//errroのポップアップ
		void errorPopup();

	private:
		void close();

	private:
		struct Check final {
			enum class ErrorType : uint8_t {
				None,
				EmptyFileName,
				EmptyFolderPath,
				AlreadyExist,
				NotFound
			};
			static std::pair<ErrorType, std::string> checkFile(const std::string& folderPath, const std::string& fileName, const std::string& extension);
			static void tryPushSlash(std::string& path);
		};
		static int inputTextCallback(ImGuiInputTextCallbackData* data);
	};
}