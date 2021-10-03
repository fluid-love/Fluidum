/*
�Ώ��ł����O�͂Ȃ�
��O�̓��O��Messenger�ɓn�������ƁC�ꗥ��"FDR::Exception::Error"�𓊂���D
FDR::Exception::Error��std::nasted_exception����h�����Ă���D
*/

#pragma once

#ifdef FluidumDraw_Vulkan
#include "../Vulkan/include.h"
#endif

#include "../Api/using.h"

#define FluidumDrawAPI

//Main
namespace FDR {

	//����������
	/*
	���C���E�B���h�E���쐬���CImGui���Z�b�g�A�b�v����.
	�����āC������function�𖈃t���[���K�؂ȏꏊ�ŌĂяo��

	�t�H���g��freetype�𗘗p����3���[�h�����.
		fa_

	���ӁFsetMessengerCallbackFunction�������C�S�Ă̊֐���mainLoop�Ȃ��ŌĂ΂Ȃ���΂Ȃ�Ȃ�

	����:
		function �Ăяo�����֐�
				 ImGui�̋@�\�͗��p�\�Ȃ̂ŁCImGui::Begin()����L�q���Ă悢�D
				 ImGui::Render��ImGui::NewFrame�͌Ăяo���Ȃ��Ă悢�DFluidumDraw���ŏ��������D

	*/

	//�t���X�N���[���E�B���h�E
	/*
	����:
		title           window��titlebar�ɕ\������镶����
		windowCloseFlag ���̕ϐ���true�ɂȂ��loop�𔲂���
	*/
	FluidumDrawAPI void mainLoop(const char* title, void(*function)(), bool* const windowCloseFlag);

}

//Function
namespace FDR {

	//Message�����������Ƃ��Ɉ��Ăяo�����
	/*
	������const std::string&�ł���CTime��LogType�t�̃��b�Z�[�W�������Ă���D

	����:
		callback �Ăяo�����֐�

	*/
	FluidumDrawAPI void setMessengerCallbackFunction(const MessengerCallbackType callback);


}

//Window
namespace FDR {

	//Window�̃T�C�Y���擾
	/*
	�߂�l: {width, height}
	*/
	FluidumDrawAPI _NODISCARD std::pair<int32_t, int32_t> getWindowSize();


	//Window�̈ʒu���擾
	/*
	�߂�l: {x, y}
	*/
	FluidumDrawAPI _NODISCARD std::pair<int32_t, int32_t> getWindowPos();


	//Window�̃T�C�Y��ύX
	/*
	����:
		width  ��
		height �c
	*/
	FluidumDrawAPI void setWindowSize(const int32_t width, const int32_t height);
	/*
	����:
		size �T�C�Y
	*/
	FluidumDrawAPI void setWindowSize(const ImVec2& size);


	//Window�̈ʒu��ύX
	/*
	����:
		x ��
		y �c
	*/
	FluidumDrawAPI void setWindowPos(const int32_t x, const int32_t y);
	/*
	����:
		pos �ʒu
	*/
	FluidumDrawAPI void setWindowPos(const ImVec2& pos);


	//Window�̃T�C�Y�ƈʒu��ύX
	/*
	����:
		width  ���̃T�C�Y
		height �c�̃T�C�Y
		x	   ���̈ʒu
		y	   �c�̈ʒu
	*/
	FluidumDrawAPI void setWindowSizePos(const int32_t width, const int32_t height, const int32_t x, const int32_t y);
	/*
	����:
		size �T�C�Y
		pos �ʒu
	*/
	FluidumDrawAPI void setWindowSizePos(const ImVec2& size, const ImVec2& pos);


	FluidumDrawAPI void miximizeWindow();

	FluidumDrawAPI void minimizeWindow();


}

//Image
namespace FDR {

	//ImGui�Ŏg�p�ł���Image���쐬���Ď擾����
	/*
	�߂�l��ImGuiImage�ŁC������ImTextureID�������Ă���D
	ImGui::Image(ImTextureID,...)�֐��̃I�[�o�[���[�h�Ƃ���ImGui::Image(FDR::ImGuiImage,...)��񋟂���D
	����͎��̂悤�Ƀ��\�[�X��������邽�߁CImGuiImage������ImTextureID(void*)���擾����Ɩ����ȃA�h���X���w���Ă��܂��\�������邩�炾�D
	������shared_ptr���g���Ă���D���\�[�X�̉���͋��L����0�ɂȂ�Ɖ�������D

	����:
		filePath �ǂݍ��މ摜�̃p�X(.png .jpeg .jpg .ppm ...)

	*/
	FluidumDrawAPI ImGuiImage createImGuiImage(const char* filePath);


}

//Font
namespace FDR {

	//�f�t�H���g�ŗp�ӂ����t�H���g�̏������T�C�Y���擾 ImGui::PushFont�p
	FluidumDrawAPI _NODISCARD ImFont* getDefaultFontMiniSize();


}