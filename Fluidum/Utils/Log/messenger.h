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

	class Messenger final {
	public:
		FluidumUtils_Class_Default_ConDestructor_Noexcept(Messenger)
			FluidumUtils_Class_Delete_CopyMove(Messenger)

	public:
		//no-throw
		template<Type Ty, typename... U>
		void add(const char* file, const Size line, const std::string& message, U&&... values) noexcept {
			try {
				std::string type = getTypeText<Ty>();

				//current time
				std::string result = getCurrentTimeString();

				//file, line
				if (file)
					((((result += '<') += file) += ',') += std::to_string(line)) += "> ";

				//make message
				std::string msg = std::format(message, std::forward<U>(values)...);

				result += type += msg;

				//no-throw
				this->wait();

				std::lock_guard lock(this->mtx);

				//no-throw
				this->callback(result);

				//no-throw
				this->message = std::move(msg);
			}
			catch (const std::exception&) {//error
				try {
					this->callback(message);
					this->message = message;
				}
				catch (const std::exception&) {
					try { std::cerr << message << std::endl; }
					catch (const std::exception&) { ; }
				}
				catch (...) {}
			}
			catch (...) {}
		}

		template<Type Ty, typename... U>
		void add_str(const std::string& message, U&&... values) noexcept {
			this->add<Ty>(nullptr, 0, message, std::forward<U>(values)...);
		}

	public:
		//call callback function
		void callCallback(const std::string& message) const;

		//The callback function must be noexcept.
		void setCallback(MessengerCallbackType callback);

		std::string getMessage();

	private:
		void wait() noexcept;

	private:
		class Lock final {
		private:
			Lock(const bool own) noexcept;
		public:
			~Lock() noexcept;

			Lock(Lock&&) noexcept;
			Lock& operator=(Lock&&) noexcept;

			FluidumUtils_Class_Delete_Copy(Lock)

		private:
			bool own;
		private:
			//no-throw
			void lock() noexcept;
			void unlock() noexcept;
		private:
			friend class Messenger;
		};

	public:
		[[nodiscard]] Lock getLock() const noexcept;

	private:
		Lock lock{ false };           //no-throw
		std::mutex mtx{};      //no-throw
		std::string message{}; //no-throw

	private:
		MessengerCallbackType callback = [](const std::string&) noexcept {};

	private://assert
		static_assert(std::is_nothrow_constructible_v<std::mutex>);
		static_assert(std::is_nothrow_constructible_v<std::string>);
	};

}