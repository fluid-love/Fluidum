#pragma once

#include "type.h"

namespace FU::Log {

	//get current time(ymd_hms_subseconds) as a string
	std::string getCurrentTimeString();


	template<Type Ty>
	const char* getTypeText() {
		if constexpr (Ty == Type::Error)
			return "Error: ";
		else if constexpr (Ty == Type::Warning)
			return "Warning: ";
		else if constexpr (Ty == Type::Info)
			return "Info: ";
		else
			return "Log: ";
	}


	class Messenger {
	public:
		Messenger() = default;
		~Messenger() = default;
		FluidumUtils_Class_Delete_CopyMove(Messenger)

	public:

		template<Type Ty, typename... U>
		void add(const std::string& message, U&&... values) {

			std::string type = getTypeText<Ty>();

			//current time
			std::string result = getCurrentTimeString();

			//make message
			std::string msg = std::format(message, std::forward<U>(values)...);

			result += type += msg;
			
			std::lock_guard lock(this->mtx);

			this->callback(result);

			this->message = std::move(result);
		}

		//call callback function
		void callCallback(const std::string& message) const;

		void setCallback(MessengerCallbackType callback);

		std::string getMessage();

	protected:
		std::mutex mtx{};
		std::string message = {};
	private:
		MessengerCallbackType callback = [](const std::string&) {};
	};

}