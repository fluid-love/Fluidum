#pragma once

#include "../common/FDtype.h"
#include "FDpiano.h"

namespace FD::Piano::Internal {

	class Dynamics final {
	private:
		using Type = ::FD::Piano::Dynamics;
	private:
		Type type = Type::n;

	public:
		void operator =(const Type type)noexcept {
			this->type = type;
		}

		operator float() const noexcept{
			return DynamicsRate[static_cast<UT>(type)];
		}
	};

	
}