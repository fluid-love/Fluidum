#pragma once

#include "type.h"

//forward
namespace FD {
	class LogWrite;
	class LogRead;
}

namespace FD::Internal::Log {

	//Log�̒��g��ۑ����Ă���
	class Data final {
	private:
		static inline std::array<std::string, LogArraySize> data{};
		static inline std::uint16_t currentIndex = 0;

		static void insert(std::string&& message);

		static inline std::atomic_bool writeFlag = false;
		static inline std::mutex mtx{};
	private:
		friend ::FD::LogWrite;
		friend ::FD::LogRead;
	};

}

namespace FD {

	//Global��1�����Ă���
	class LogWrite final {
	public:
		LogWrite();
		~LogWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(LogWrite)

	public:
		template<Log::Type Ty, typename... U>
		void add(const std::string& message, U&&... values) {

			std::string type = FU::Log::getTypeText<Ty>();

			//���݂̎���
			std::string result = FU::Log::getCurrentTimeString();

			//���b�Z�[�W�{��
			std::string msg = std::format(message, std::forward<U>(values)...);

			result += type += msg;

#ifndef NDEBUG
			std::cout << result << std::endl;
#endif

			Internal::Log::Data::insert(std::move(result));
		}

	private:
		//�t�@�C���� jthread����ɔj�����Ă͂����Ȃ�
		//"FluidumLog/���݂̎���.log"
		const std::string filePath;

		void file(const bool all = true);
		void write();

		std::jthread writeLogThread{};

	};


	class LogRead final {
	public:
		LogRead(Internal::PassKey) {}
		~LogRead() = default;
		FluidumUtils_Class_Delete_CopyMove(LogRead)

	public:
		//�Ō�ɒǉ����ꂽ���b�Z�[�W���擾����
		_NODISCARD std::string getLatestMessage() const;
	};

}