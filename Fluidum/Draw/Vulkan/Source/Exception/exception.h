#pragma once

#include "../Common/type.h"
#include "glfw.h"
#include "../Common/fkm.h"

namespace FVK::Internal::Exception {

	[[noreturn]] void throwNotInitialized(const char* message);
	[[noreturn]] void throwFailedToCreate(const char* message);
	[[noreturn]] void throwFailedToInitialize(const char* message);
	[[noreturn]] void throwFailedToAllocate(const char* message);
	[[noreturn]] void throwFailedToDestroy(const char* message);
	[[noreturn]] void throwNotFound(const char* message);
	[[noreturn]] void throwCollisionOfKeys(const char* key);
	[[noreturn]] void throwNotSupported(const char* message);

}

namespace FVK::Exception {


	//初期化されていない
	class NotInitialized final : public std::exception {
	private:
		using Base = std::exception;
	public:
		explicit NotInitialized(const std::string& message) : Base(message.c_str()) {}
		explicit NotInitialized(const char* message) : Base(message) {}
	};

	class FailedToCreate final : public std::exception {
	private:
		using Base = std::exception;
	public:
		explicit FailedToCreate(const std::string& message) : Base(message.c_str()) {}
		explicit FailedToCreate(const char* message) : Base(message) {}
	};

	using FailedToInitialize = FailedToCreate;
	using FailedToAllocate = FailedToCreate;

	//破壊に失敗
	class FailedToDestroy final : public std::exception {
	private:
		using Base = std::exception;
	public:
		explicit FailedToDestroy(const std::string& message) : Base(message.c_str()) {}
		explicit FailedToDestroy(const char* message) : Base(message) {}
	};

	using FailedToFree = FailedToDestroy;



	//サポートされていない
	class NotSupported final : public std::exception {
	private:
		using Base = std::exception;
	public:
		explicit NotSupported(const std::string& message) : Base(message.c_str()) {}
		explicit NotSupported(const char* message) : Base(message) {}
	};

	using ::FKM::Exception::NotFound;
	using ::FKM::Exception::CollisionOfKeys;
	using ::FKM::Exception::FailedToErase;
}