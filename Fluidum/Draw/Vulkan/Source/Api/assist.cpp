#include "FVKassist.h"

void FVK::Internal::API::checkManagerEmpty() {
	if (!Internal::GManager)
		Internal::Exception::throwNotInitialized("FluidumVK is not initialized.");
}
