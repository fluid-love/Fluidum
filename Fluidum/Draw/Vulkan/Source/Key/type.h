#pragma once

#include "../Common/fvktype.h"
#include "../Common/fkm.h"

namespace FVK::Internal::Key {
	using ::FKM::OrderKey;
	using ::FKM::IndexKey;
	using ::FKM::CharKey;
	using ::FKM::StrKey;
	using ::FKM::SelectKey;
}

//forward declaration
namespace FVK::Internal {
	namespace Key {
		template<FvkType>
		class RequiredDataKeys;

		class Converter;

		template<FvkType, ::FKM::Concept::IsKeyType>
		class Key;
	}
	namespace Manager {
		class Manager;
	}
}

namespace FVK::Internal::Key {

	template<typename T>
	concept IsKey = FU::Concept::IsInstanceOfNonTypeType<T, ::FVK::Internal::Key::Key>;

	template<typename T>
	concept IsKeyType = ::FKM::Concept::IsKeyType<T>;

}