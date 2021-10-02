#pragma once

#include <FKMinclude.h>
#include "../Common/FVKfvktype.h"
#include "../Common/FVKfkm.h"

namespace FVK::Internal::Key {
	using StringKeyType = std::string;
	using CharKeyType = const char*;
	using SelectKeyType = FKM::SelectKey;
	using IndexKeyType = FKM::IndexKey;
}

namespace FVK::Internal::Key {
	/*Key�̌^��4��*/

	//std::string
	template<typename T>
	concept IsStringKey = std::same_as<T, std::string>;

	//const char*
	template<typename T>
	concept IsCharKey = std::same_as<T, const char*>;

	//SelectKey->�͂��߁@�Ƃ��@�������@�Ƃ�
	template<typename T>
	concept IsSelectKey = std::same_as<T, FKM::SelectKey>;

	//IndexKey->���� Window��ǉ�������C�����0�Ƃ��āC�ǉ����Ă�������+1���Ă���
	template<typename T>
	concept IsIndexKey = std::same_as<T, FKM::IndexKey>;

	//Key�ɋ�������
	template<typename T>
	concept IsKeyType =
		IsStringKey<T> ||
		IsCharKey<T> ||
		IsSelectKey<T> ||
		IsIndexKey<T>;

}

//�O���錾
namespace FVK::Internal {
	namespace Key {
		template<FvkType>
		class RequiredDataKeysBase;

		class Converter;

		template<FvkType, IsKeyType>
		class Key;
	}
	namespace Manager {
		class Manager;
	}
}

namespace FVK::Internal::Key {

	template<typename T>
	concept IsKey = FU::Concept::IsInstanceOfNonTypeType<T, ::FVK::Internal::Key::Key>;


}