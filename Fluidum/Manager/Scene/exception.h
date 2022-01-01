#pragma once

#include <exception>
#include <string>

namespace FS::Exception {

	class AlreadyAdded final : public std::exception {
	private:
		using Base = std::exception;
	public:
		explicit AlreadyAdded(const std::string& message) : Base(message.c_str()) {}
	};

	class AlreadyDeleted final : public std::exception {
	private:
		using Base = std::exception;
	public:
		explicit AlreadyDeleted(const std::string& message) : Base(message.c_str()) {}
	};

	namespace Internal {
		template<typename Scene>
		[[noreturn]] void throwAlreadyAdded() {
			std::string message = typeid(Scene).name();
			message += " is already added.";
			throw AlreadyAdded(message);
		}

		template<typename Scene>
		[[noreturn]] void throwAlreadyDeleted() {
			std::string message = typeid(Scene).name();
			message += " is already deleted.";
			throw AlreadyDeleted(message);
		}
	}

}