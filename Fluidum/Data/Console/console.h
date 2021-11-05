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
		FluidumUtils_Class_Delete_ConDestructor(Data)
			FluidumUtils_Class_Delete_CopyMove(Data)

	private:
		static constexpr uint16_t LimitMaxLineSize = 20000;
		static inline std::vector<::FD::Console::Info> texts{};
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
		FluidumUtils_Class_Delete_CopyMove(ConsoleWrite)


	public:

		template<typename String>
		requires(FU::Concept::IsStdString<String>) void add(String&& message) {
			Console::Info info{
				std::forward<String>(message)
			};
			std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);

			using namespace Console::Internal;
			if (Data::texts.size() > Data::LimitMaxLineSize)
				Data::texts.erase(Data::texts.begin(), Data::texts.begin() + 500);
			Data::texts.emplace_back(std::move(info));
		}

		template<typename String, typename... T>
		requires((sizeof...(T) > 0) && FU::Concept::IsStdString<String>)
			void add(String&& message, T&&... values) {

			//formatÇÃà¯êîÇÕ&ÇÃÇ›
			std::string result = std::format(message, std::forward<T>(values)...);

			Console::Info info{
				std::move(result)
			};
			std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);
		
			using namespace Console::Internal;
			if (Data::texts.size() > Data::LimitMaxLineSize)
				Data::texts.erase(Data::texts.begin(), Data::texts.begin() + 500);
			Data::texts.emplace_back(std::move(info));
		}



	};

	class ConsoleRead final {
	public:
		explicit ConsoleRead(Internal::PassKey) {};
		~ConsoleRead() = default;
		FluidumUtils_Class_Delete_CopyMove(ConsoleRead)

	public:
		//return !out_of_range, text
		std::pair<bool, Console::Info> get(const std::size_t index) const;
	};

}