#include "exception.h"

std::string FKM::Exception::Internal::makeCollisionOfKeysMessage(const char* key) {
	std::string result = "Collision Of Keys. key == ";
	(result += key) += '.';
	return result;
}

void FKM::Exception::Internal::throwCollisionOfKeys(const char* key) {
	const std::string message = makeCollisionOfKeysMessage(key);
	throw CollisionOfKeys(message);
}

void FKM::Exception::Internal::throwReachIndexLimit() {
	throw CollisionOfKeys("Reach Index Limit.");
}

void FKM::Exception::Internal::throwNotFound() {
	throw NotFound("Not Found.");
}

void FKM::Exception::Internal::throwFailedToErase(const char* key) {
	std::string message = "Failed to erase key. ";
	(message += key) += '.';
	throw NotFound(message);
}
