#include "FVKexception.h"

void FVK::Internal::Exception::throwNotInitialized(const char* message) {
	throw ::FVK::Exception::NotInitialized(message);
}

void FVK::Internal::Exception::throwFailedToCreate(const char* message) {
	throw ::FVK::Exception::FailedToCreate(message);
}

void FVK::Internal::Exception::throwFailedToInitialize(const char* message) {
	throw ::FVK::Exception::FailedToInitialize(message);
}

void FVK::Internal::Exception::throwFailedToAllocate(const char* message) {
	throw ::FVK::Exception::FailedToAllocate(message);
}

void FVK::Internal::Exception::throwFailedToDestroy(const char* message) {
	throw ::FVK::Exception::FailedToDestroy(message);
}

void FVK::Internal::Exception::throwNotFound(const char* message) {
	throw ::FVK::Exception::NotFound(message);
}

void FVK::Internal::Exception::throwCollisionOfKeys(const char* key) {
	std::string message = "Keys are collision. key name is ";
	(message += key) += '.';
	throw ::FVK::Exception::CollisionOfKeys(message);
}

void FVK::Internal::Exception::throwNotSupported(const char* message) {
	throw ::FVK::Exception::CollisionOfKeys(message);
}
