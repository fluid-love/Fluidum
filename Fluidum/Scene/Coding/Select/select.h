#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS {

	class CodingSelect final : public Scene {
	public:
		explicit CodingSelect(const FD::ProjectRead* const projectRead, FD::ProjectWrite* const projectWrite, const FD::GuiRead* const guiRead, FD::GuiWrite* const guiWrite);
		void Constructor(FD::ProjectRead, FD::ProjectWrite, FD::GuiRead, FD::GuiWrite);

		~CodingSelect() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::ProjectRead* const projectRead;
		FD::ProjectWrite* const projectWrite;
		const FD::GuiRead* const guiRead;
		FD::GuiWrite* const guiWrite;

		FD::Text::CodingSelect text{};

	private:

		const FDR::ImGuiImage newImage;
		const FDR::ImGuiImage openImage;

		struct {
			ImVec2 windowSize{};
			ImVec2 windowPos{};

			bool isOpenWindowHovered = false;
			bool isNewFileWindowHovered = false;

		}style;

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

		struct {
			std::string errorPopupMessage{};
		}error;

	private:
		//新規作成
		void create();


		//最近つかったもの
		void quick();
		void selectTemplate(const ImVec2& size);

		//開く
		void right();
		void open(const ImVec2& size);
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
	};
}