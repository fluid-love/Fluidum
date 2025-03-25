#pragma once

#include "../Base/base.h"

namespace FDR::Internal {

	//この関数を別スレッドで呼び出してmainスレッドと交換
	void mainLoop(void(*function)(), const bool* const flag);

}