#pragma once

#include "type.h"

//forward
namespace FD {
	class LogWrite;
	class LogRead;
}

namespace FD::Log::Internal {

	class Data final {
	private:
		static inline std::array<std::string, Limits::LogArraySize> data{};
		static inline UIF16 currentIndex = 0;

		static void insert(std::string&& message);
		static void insert(const std::string& message);

		static inline std::atomic_bool writeFlag = false;
		static inline std::mutex mtx{};

	private:
		friend ::FD::LogWrite;
		friend ::FD::LogRead;

	};

}

namespace FD {

	class LogWrite final {
	public:
		LogWrite() noexcept;
		~LogWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(LogWrite)

	public:
		enum class Exception : UT {
			FailedToAdd
		};

	public:
		/*
		Exception:
			FailedToAdd
		*/
		//strong
		void add(const std::string& message);

	public:
		//no-throw
		void requestStop() noexcept;

		[[nodiscard]] bool threadJoinable() const noexcept;

	private:
		//filepath
		//Don't destroy it before jthread.
		//"FluidumLog/current_time.log"
		const std::string filePath;

		void file(const bool all = true) noexcept;
		void write() noexcept;

	private:
		std::jthread writeLogThread{};

	};

	class LogRead final {
	public:
		LogRead(Internal::PassKey) {}
		~LogRead() = default;
		FluidumUtils_Class_Delete_CopyMove(LogRead)

	public:
		//Get the last message added.
		[[nodiscard]] std::string getLatestMessage() const;

	};

}