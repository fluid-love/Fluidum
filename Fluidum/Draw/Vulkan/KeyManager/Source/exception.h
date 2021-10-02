#pragma once

#include "type.h"

namespace Fluidum::KeyManager::Exception {

	class CollisionOfKeys : public std::exception {
	private:
		using Base = std::exception;

	public:
		explicit CollisionOfKeys(const std::string& message)
			:Base(message.c_str()) {}
	};

	class ReachIndexLimit : public std::exception {
	private:
		using Base = std::exception;

	public:
		explicit ReachIndexLimit(const char* message)
			:Base(message) {}
	};

	class NotFound : public std::exception {
	private:
		using Base = std::exception;

	public:
		explicit NotFound(const std::string& message)
			:Base(message.c_str()) {}
		explicit NotFound(const char* message)
			:Base(message) {}
	};

	class FailedToErase : public std::exception {
	private:
		using Base = std::exception;

	public:
		explicit FailedToErase(const std::string& message)
			:Base(message.c_str()) {}
		explicit FailedToErase(const char* message)
			:Base(message) {}
	};


}

namespace Fluidum::KeyManager::Exception::Internal {

	_NODISCARD std::string makeCollisionOfKeysMessage(const char* key);
	[[noreturn]] void throwCollisionOfKeys(const char* key);

	[[noreturn]] void throwReachIndexLimit();

	[[noreturn]] void throwNotFound();

	[[noreturn]] void throwFailedToErase(const char* key);

	

}