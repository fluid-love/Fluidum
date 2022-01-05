#include "log.h"

void FD::Log::Internal::Data::insert(std::string&& message) {
	std::lock_guard<std::mutex> lock(mtx); //exception

	//no-throw
	data[currentIndex] = std::move(message);

	currentIndex++;
	if (currentIndex >= Limits::LogArraySize) {
		writeFlag.store(true);
		currentIndex = 0;
	}
}

void FD::Log::Internal::Data::insert(const std::string& message) {
	std::lock_guard<std::mutex> lock(mtx); //exception
	data[currentIndex] = message;		   //exception

	//no-throw
	currentIndex++;

	if (currentIndex >= Limits::LogArraySize) {
		writeFlag.store(true);
		currentIndex = 0;
	}
}

namespace FD {
	void cerr(const char* message) noexcept {
		try {
			std::cerr << message << std::endl;
		}
		catch (...) {
			;
		}
	}
}

void FD::LogWrite::file(const bool all) noexcept {

	//no directory
	try {
		if (!std::filesystem::is_directory(Internal::Log::FluidumLogFolderDirectory)) {
			std::filesystem::create_directory(Internal::Log::FluidumLogFolderDirectory);
		}
	}
	catch (...) {
		cerr("Failed to write log.");
		return;
	}

	try {
		std::ofstream ofs(filePath, std::ios::app);

		if (!ofs)
			throw std::runtime_error("");

		using namespace Internal::Log;

		if (all) {
			std::lock_guard<std::mutex> lock(Log::Internal::Data::mtx);
			for (const auto& x : Log::Internal::Data::data) {
				ofs.write(x.data(), x.size() * sizeof(std::string::value_type));
			}
		}
		else {
			std::lock_guard<std::mutex> lock(Log::Internal::Data::mtx);
			for (UIF16 i = 0; i < Log::Internal::Data::currentIndex; i++) {
				ofs.write(Log::Internal::Data::data[i].data(), Log::Internal::Data::data[i].size() * sizeof(std::string::value_type));
			}
			const std::string last = "All the logs were successfully output to a file.";
			ofs.write(last.data(), last.size() * sizeof(std::string::value_type));
		}
	}
	catch (...) {
		cerr("Failed to write log.");
		return;
	}

}

void FD::LogWrite::write() noexcept {
	bool breakFlag = false;

	while (true) {
		//wait
		while (!Log::Internal::Data::writeFlag.load()) {
			const bool request = this->writeLogThread.get_stop_token().stop_requested();
			if (request)//finish. Write the remaining logs to a file.
				breakFlag = true;
		}

		if (!breakFlag) {
			this->file();
		}
		else {//Write the remaining logs to a file.
			this->file(false);
			break;
		}

		Log::Internal::Data::writeFlag.store(false);

		if (breakFlag)
			break;
	}

}

namespace FD::Internal::Log {

	std::string makeFilePath() {
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
		ss << FluidumLogFolderDirectory << ymd.year() << '_' << ymd.month() << '_' << ymd.day() << '_'
			<< time.hours().count() << '_' << time.minutes().count() << '_' << time.seconds().count() << '_' << time.subseconds().count() << ".log";
		return ss.str();
	}
}

FD::LogWrite::LogWrite() noexcept
	: filePath(Internal::Log::makeFilePath())
{
	try {
		//If there is no directory, create.
		if (!std::filesystem::is_directory(Internal::Log::FluidumLogFolderDirectory)) {
			if (!std::filesystem::create_directory(Internal::Log::FluidumLogFolderDirectory))
				throw -1;
		}

		this->writeLogThread = std::jthread(&LogWrite::write, this);
	}
	catch (...) {
		try { 
			std::cerr << "[Serious Error] Failed to construct ::FD::LogWrite.";
		}
		catch (...) {
			;
		}
		std::abort();
	}
}

void FD::LogWrite::add(const std::string& message) {
	try {
		Log::Internal::Data::insert(message);
	}
	catch (...) {
		throw Exception::FailedToAdd;
	}
}

void FD::LogWrite::requestStop() noexcept {
	this->writeLogThread.request_stop();
}

bool FD::LogWrite::threadJoinable() const noexcept {
	return this->writeLogThread.joinable();
}

std::string FD::LogRead::getLatestMessage() const {
	std::lock_guard<std::mutex> lock(Log::Internal::Data::mtx);
	return Log::Internal::Data::data[Log::Internal::Data::currentIndex == 0 ? Log::Limits::LogArraySize - 1 : Log::Internal::Data::currentIndex - 1];
}
