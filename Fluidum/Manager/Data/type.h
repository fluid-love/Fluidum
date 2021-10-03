#pragma once

#include "../../Utils/include.h"

namespace FD::Internal {
	class ManagerPassKey;
}

//���̐�����f�[�^�ɉۂ�
namespace FD {

	//ManagerPassKey���R���X�g���N�^�̈����ł��邱��
	template<typename T>
	concept IsConstructibleFromManagerPassKey = std::constructible_from<T,const Internal::ManagerPassKey&>;

	//Copy��Move���֎~����Ă��邱��
	template<typename T>
	concept IsNotCopyMoveAble = !std::copyable<T> && !std::movable<T>;

	//default�\�z�s��
	template<typename T>
	concept IsNotDefaultConstructible = !std::is_default_constructible_v<T>;

	//�]�v�ȏC���^�łȂ�
	template<typename T>
	concept IsNotCVR =
		!std::is_const_v<T> &&
		!std::is_volatile_v<T> &&
		!std::is_reference_v<T>;

	//�N���X�ł��邱��
	template<typename T>
	concept IsClass = FU::Concept::IsClass<T>;

	//�|�C���^������ �S�Ă𖞂������̂������󂯓������
	template<typename T>
	concept IsDataAble =
		(std::is_pointer_v<T> && IsClass<std::remove_pointer_t<T>>) || (
			IsConstructibleFromManagerPassKey<T> &&
			IsNotCopyMoveAble<T> &&
			IsNotDefaultConstructible<T> &&
			IsNotCVR<T> &&
			IsClass<T>);

}