#pragma once

#include "../Common/common.h"

//forward
namespace FD {
	class ConsoleWrite;
	class ConsoleRead;
}

namespace FD::Console {
	struct Info final {
		std::string message{};
	};
}

namespace FD::Console::Internal {
	class Data final {
	private:
		FluidumUtils_Class_Delete_ConDestructor(Data);
			FluidumUtils_Class_Delete_CopyMove(Data);

	private:
		static inline std::vector<::FD::Console::Info> texts{};
		static inline bool busy = false;
		static inline std::mutex mtx{};
	private:
		friend ::FD::ConsoleWrite;
		friend ::FD::ConsoleRead;
	};
}

namespace FD {

	class ConsoleWrite final {
	public:
		explicit ConsoleWrite(Internal::PassKey) {
			Console::Internal::Data::texts.reserve(2000);
		};
		~ConsoleWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(ConsoleWrite);


	public:

		template<typename String>
		requires(FU::Concept::IsStdString<String>) void push(String&& message) const{
			Console::Info info{
				std::forward<String>(message)
			};
			std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);

			using namespace Console::Internal;
			if (Data::texts.size() > Console::Limits::Lines)
				Data::texts.erase(Data::texts.begin(), Data::texts.begin() + 500);
			Data::texts.emplace_back(std::move(info));
		}

		template<typename String, typename... T>
		requires((sizeof...(T) > 0) && FU::Concept::IsStdString<String>)
			void push(String&& message, T&&... values) const {

			//formatÇÃà¯êîÇÕ&ÇÃÇ›
			std::string result = std::format(message, std::forward<T>(values)...);

			Console::Info info{
				std::move(result)
			};
			std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);

			using namespace Console::Internal;
			if (Data::texts.size() > Console::Limits::Lines)
				Data::texts.erase(Data::texts.begin(), Data::texts.begin() + 500);
			Data::texts.emplace_back(std::move(info));
		}

		void push_input(const std::string& message) const;

		void push_input(std::string&& message) const;

		template<typename... T>
		void push_input(const std::string& message, T&&... values) const {
			std::string msg = ">" + message;
			this->push(std::move(msg), std::forward<T>(values)...);
		}

		template<typename... T>
		void push_input(std::string&& message, T&&... values) const {
			message.insert(message.begin(), '>');
			this->push(std::move(message), std::forward<T>(values)...);
		}

	public:
		void busy(const bool val) const;

	};

	class ConsoleRead final {
	public:
		explicit ConsoleRead(Internal::PassKey) {};
		~ConsoleRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ConsoleRead);

	public:
		//return !out_of_range, text
		std::pair<bool, Console::Info> get(const std::size_t index) const;

	public:
		[[nodiscard]] bool busy() const;
		[[nodiscard]] std::size_t size() const;
	};

}