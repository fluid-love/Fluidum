#pragma once

#include "../Common/common.h"

namespace FS {

	class Title final : public Scene {
	public:
		explicit Title(
			const FD::ProjectRead* const projectRead,
			FD::GuiWrite* const guiWrite,
			const FD::GuiRead* const guiRead
		);
		void Constructor(FD::ProjectRead, FD::GuiWrite, FD::GuiRead);

		~Title() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Title)

	public:
		virtual void call() override;

	private:
		const FD::ProjectRead* const projectRead;
		FD::GuiWrite* const guiWrite;
		const FD::GuiRead* const guiRead;

		FD::Text::Title text{};

	private:
		std::once_flag once{};
		const std::array<FD::Project::HistoryInfo, 50> recentProjectInfos;

	private:
		//constructor�͂܂�FluidumDraw�̏��������I����ĂȂ��̂Ń��[�v���ň�񂾂����
		void writeGuiData();

		//�}�E�X���N���b�N������V�[����؂�ւ�
		void changeScene();

		//�^�C�g���摜�̕`��
		void drawTitleImage();

		//select recent project
		void selectProject();

		void newProject();
	private:

		//1920�~1080 title�摜
		//�摜�f�[�^
		//Title�V�[���͓��ʂŁC�R���X�g���N�^���_��FluidumDraw�͏���������Ă��Ȃ��̂�optional�Œx������D
		std::optional<FDR::ImGuiImage> image = std::nullopt;

		//recent
		std::optional<FDR::ImGuiImage> projectIcon = std::nullopt;

		//project window��hovered����Ă���Ύ��ɐi�܂��Ȃ�
		bool isSelectProjectHovered = false;

		//style
		struct {
			const ImVec2 imageHalfSize = ImVec2(960, 540);
			ImVec2 windowPos{};

			ImVec2 selectWindowPos{};
			ImVec2 selectWindowSize{};

		}style;

		//���Ԃ�������̂ł������ŏ������Ă���
		std::optional<std::vector<FDR::ImGuiImage>> leftBarImages = std::nullopt;

	};
}