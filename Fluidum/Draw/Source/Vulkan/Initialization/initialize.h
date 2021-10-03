#pragma once

#include "../Base/base.h"

namespace FDR::Internal::Initialization {

	struct FullScreenWithBarInfo {
		const char* title = "";


	};

	//初期化
	/*
	最低限描写に必要なアイテムとImGuiが使えるようにする
	ImGuiにはフォントとアイコンをロード
	*/
	void initialize(const FullScreenWithBarInfo& info);

}