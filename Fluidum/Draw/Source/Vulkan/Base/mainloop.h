#pragma once

#include "../Base/base.h"

namespace FDR::Internal {

	//���̊֐���ʃX���b�h�ŌĂяo����main�X���b�h�ƌ���
	void mainLoop(void(*function)(),bool* const flag);

}