#include "exception.h"

void FVK::Internal::Exception::throwNotInitialized() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::NotInitialized);
}

void FVK::Internal::Exception::throwFailedToCreate() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::FailedToCreate);
}

void FVK::Internal::Exception::throwFailedToDestroy() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::FailedToDestroy);
}

void FVK::Internal::Exception::throwNotFound() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::NotFound);
}

void FVK::Internal::Exception::throwNotSupported() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::NotSupported);
}

void FVK::Internal::Exception::throwCollisionOfKeys() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::CollisionOfKeys);
}

void FVK::Internal::Exception::throwUnexpected() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::Unexpected);
}

void FVK::Internal::Exception::throwUnknown() {
	throw ::FVK::Exception::Error(::FVK::Exception::ErrorType::Unknown);
}
