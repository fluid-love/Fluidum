#include "task.h"

void FD::TaskWrite::add(const Task::Message& message) const {
	using namespace Task::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	Data::eraseInvalidElms();
	Data::info.emplace_back(message);
}

void FD::TaskWrite::setNotice(const Task::Message& message) const {
	using namespace Task::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	Data::notice = message;
}

void FD::Task::Internal::Data::eraseInvalidElms() {
	std::erase_if(Data::info, [](auto& x) {return !x.expired(); });
}
