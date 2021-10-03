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
		bool isFileSelected = false;//color�p


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
		//�V�K�쐬
		void create();


		//�ŋ߂���������
		void quick();
		void selectTemplate(const ImVec2& size);

		//�J��
		void right();
		void open(const ImVec2& size);
		void newFile(const ImVec2& size);

		//�E��(�N�C�b�N�ɂ�����p�X�ƃt�@�C��������)
		void bottomRight();

		//��
		void bottom();

		void setEmptyFile();

		//�N�C�b�N�ō��ꍇ�̓v���W�F�N�g�t�H���_��Code�t�H���_�ɓ����
		void createNewFileQuick();
		_NODISCARD bool checkQuickInfo();

		//errro�̃|�b�v�A�b�v
		void errorPopup();
	};
}