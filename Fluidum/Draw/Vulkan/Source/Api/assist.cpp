#include "assist.h"

void FVK::Internal::Api::checkManagerEmpty() {
	if (!Internal::GManager)
		Internal::Exception::throwNotInitialized();
}

void FVK::Internal::Api::stdExceptionMessage(const char* funcName, const char* what) noexcept {
	GMessenger.add<FU::Log::Type::None>(nullptr, 0, "Exception(std::exception) was thrown. Caught:{}.", funcName);
}


































