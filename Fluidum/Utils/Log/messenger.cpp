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

void FU::Log::Messenger::callCallback(const std::string& message) const{
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
