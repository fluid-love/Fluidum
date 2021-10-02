#include "exception.h"

std::string Fluidum::KeyManager::Exception::Internal::makeCollisionOfKeysMessage(const char* key) {
	std::string result = "Collision Of Keys. key == ";
	(result += key) += '.';
	return result;
}

void Fluidum::KeyManager::Exception::Internal::throwCollisionOfKeys(const char* key) {
	const std::string message = makeCollisionOfKeysMessage(key);
	throw CollisionOfKeys(message);
}

void Fluidum::KeyManager::Exception::Internal::throwReachIndexLimit() {
	throw CollisionOfKeys("Reach Index Limit.");
}

void Fluidum::KeyManager::Exception::Internal::throwNotFound() {
	throw NotFound("Not Found.");
}

void Fluidum::KeyManager::Exception::Internal::throwFailedToErase(const char* key) {
	std::string message = "Failed to erase key. ";
	(message += key) += '.';
	throw NotFound(message);
}
