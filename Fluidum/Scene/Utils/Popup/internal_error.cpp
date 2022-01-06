#include "internal_error.h"

FS::Utils::InternalError::InternalError() {
	FluidumScene_Log_Constructor(::FS::Utils::InternalError);
}

FS::Utils::InternalError::~InternalError() noexcept {
	FluidumScene_Log_Destructor(::FS::Utils::InternalError);
}

void FS::Utils::InternalError::call() {
	assert(false);
}
