#pragma once

#include "../Base/base.h"

namespace FDR::Internal::Initialization {

	struct FullScreenWithBarInfo {
		const char* title = "";


	};

	//������
	/*
	�Œ���`�ʂɕK�v�ȃA�C�e����ImGui���g����悤�ɂ���
	ImGui�ɂ̓t�H���g�ƃA�C�R�������[�h
	*/
	void initialize(const FullScreenWithBarInfo& info);

}