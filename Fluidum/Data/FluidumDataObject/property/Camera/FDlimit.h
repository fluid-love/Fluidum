#pragma once

#include <iostream>
#include "../../common/FDtype.h"

/*
S        : �ς��邱�Ƃ̂ł��Ȃ����
W        : �x��
D        : �f�t�H���g�l
�����Ȃ� : ���[�U�[�ς��邱�Ƃ̏o������
*/

namespace FD::Object::Camera {
	using Speed = float;
}

namespace FD::Object::Camera {

	class Limit final {
	public:
		static constexpr Speed SpeedMaxS = 50.0f; //�J�����ړ��̍ő�l 50.0f/1frame
		static constexpr Speed SpeedMinS = 0.0f;  //�J�����ړ��̍ŏ��l 0.0f/1frame
		static constexpr Speed SpeedD = 0.03f;    //�J�����ړ��̏����l 0.03f/1frame

	public:


	};


}