#include "messenger.h"

std::string FU::Log::getCurrentTimeString() {

	auto time1 = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time();
	auto time2 = std::chrono::floor<std::chrono::days>(time1);
	std::chrono::year_month_day ymd(time2);

	std::chrono::hh_mm_ss time(std::chrono::floor< std::chrono::milliseconds>(time1 - time2));
	auto y = ymd.year();
	auto m = ymd.month();
	auto d = ymd.day();
	auto h = time.hours();
	auto M = time.minutes();
	auto s = time.seconds();
	auto ms = time.subseconds();

	std::stringstream ss;
	ss << "[" << ymd.year() << '-' << ymd.month() << '-' << ymd.day() << ' '
		<< time.hours().count() << ':' << time.minutes().count() << ':' << time.seconds().count() << '.' << time.subseconds().count() << "] ";
	return ss.str();
}

void FU::Log::Messenger::callCallback(const std::string& message) const {
	this->callback(message);
}

void FU::Log::Messenger::setCallback(MessengerCallbackType callback) {
	std::lock_guard lock(this->mtx);
	this->callback = callback;
}

std::string FU::Log::Messenger::getMessage() {
	std::lock_guard lock(this->mtx);
	return this->message;
}

namespace FU::Log {
	std::thread::id GThreadID{};
	std::atomic_bool GWait{};
}

FU::Log::Messenger::Lock::Lock(const bool own) noexcept : own(own) {
	static_assert(noexcept(GThreadID = std::this_thread::get_id()));
	GThreadID = std::this_thread::get_id();
}

FU::Log::Messenger::Lock::~Lock() noexcept {
	if (this->own)
		this->unlock();
}

FU::Log::Messenger::Lock::Lock(Lock&&) noexcept {
	this->own = true;
}

FU::Log::Messenger::Lock& FU::Log::Messenger::Lock::operator=(Lock&&) noexcept {
	this->own = true;
	return *this;
}

void FU::Log::Messenger::Lock::lock() noexcept {
	GWait.store(true);
}

void FU::Log::Messenger::Lock::unlock() noexcept {
	GWait.store(false);
}

void FU::Log::Messenger::wait() noexcept {
	if (GThreadID == std::this_thread::get_id())
		return;

	while (true) {
		if (GWait.load()) //wait
			;
		else
			break;
	}

}

FU::Log::Messenger::Lock FU::Log::Messenger::getLock() const noexcept {
	return Lock(false);
}




