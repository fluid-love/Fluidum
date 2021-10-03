#pragma once

#include "../Common/common.h"

namespace FD::Style {

	enum class StyleType : uint8_t {
		Default,
		Dark,
		Light
	};
}

namespace FD::Internal::Style {



}

namespace FD {

	//�͂��߂��珊������D
	//�ċN���ŐF�̕ύX���\�ɂ���D
	class StyleColorRead final {
	public:
		StyleColorRead(Internal::PassKey);
		~StyleColorRead() = default;
		FluidumUtils_Class_Delete_CopyMove(StyleColorRead)

#ifdef FluidumData_DebugMode
			StyleColorRead() = default;
#endif 

	public:
		_NODISCARD Style::StyleType getType() const noexcept;

	private:
		const Style::StyleType type;

	};


	class StyleColorWrite final {
	public:
		StyleColorWrite(Internal::PassKey) {}
		~StyleColorWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(StyleColorWrite)

#ifdef FluidumData_DebugMode
			StyleColorWrite() = default;
#endif 


	};

}