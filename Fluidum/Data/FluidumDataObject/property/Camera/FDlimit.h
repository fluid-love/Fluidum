#pragma once

#include <iostream>
#include "../../common/FDtype.h"

/*
S        : 変えることのできない上限
W        : 警告
D        : デフォルト値
何もない : ユーザー変えることの出来る上限
*/

namespace FD::Object::Camera {
	using Speed = float;
}

namespace FD::Object::Camera {

	class Limit final {
	public:
		static constexpr Speed SpeedMaxS = 50.0f; //カメラ移動の最大値 50.0f/1frame
		static constexpr Speed SpeedMinS = 0.0f;  //カメラ移動の最小値 0.0f/1frame
		static constexpr Speed SpeedD = 0.03f;    //カメラ移動の初期値 0.03f/1frame

	public:


	};


}