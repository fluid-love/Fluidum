#include "internal_error.h"

FS::Utils::InternalError::InternalError() {
	FluidumScene_Log_Constructor("Utils::InternalError");


}

FS::Utils::InternalError::~InternalError() noexcept {
	FluidumScene_Log_Destructor_("Utils::InternalError")
}

void FS::Utils::InternalError::call() {
	assert(false);
}
