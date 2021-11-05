#pragma once

#include "../Common/common.h"

namespace FD {
	class TaskWrite;
	class TaskRead;
}

namespace FD::Task {
	struct Info final {
		std::string message{};
	};

	using Message = std::shared_ptr<Info>;
}

namespace FD::Task::Internal {
	class Data final {
	private:
		static inline std::vector<std::weak_ptr<Task::Info>> info{};
		static inline std::weak_ptr<Task::Info> notice{};

		static inline std::mutex mtx{};

	private:
		static void eraseInvalidElms();
	private:
		friend class TaskWrite;
		friend class TaskRead;
	};
}

namespace FD {

	class TaskWrite final {
	public:
		explicit TaskWrite(Internal::PassKey) noexcept {};
		~TaskWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(TaskWrite)

	public:
		void add(const Task::Message& message) const;

		void setNotice(const Task::Message& message) const;

	};

	class TaskRead final {
	public:
		explicit TaskRead(Internal::PassKey) noexcept {};
		~TaskRead() = default;
		FluidumUtils_Class_Delete_CopyMove(TaskRead)

	public:
		template<typename Function>
		void for_each(Function func) const {
			using namespace Task::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			bool erase = false;
			for (const auto& x : Data::info) {
				Task::Message shared = x.lock();
				if (shared)
					func(shared.get());
				else
					erase = true;;
			}

			if (!erase)
				return;

			Data::eraseInvalidElms();
			return;
		}

		//return invalid -> false
		template<typename Function>
		bool notice(Function func) const {
			using namespace Task::Internal;
			std::lock_guard<std::mutex> lock(Data::mtx);

			Task::Message info = Data::notice.lock();
			if (info) {
				func(info.get());
				return true;
			}

			return false;
		}

	};

}