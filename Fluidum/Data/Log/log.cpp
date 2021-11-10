#include "log.h"

void FD::Internal::Log::Data::insert(std::string&& message) {
	std::lock_guard<std::mutex> lock(mtx);
	data[currentIndex] = std::move(message);

	currentIndex++;
	if (currentIndex >= LogArraySize) {
		writeFlag.store(true);
		currentIndex = 0;
	}
}

void FD::Internal::Log::Data::insert(const std::string& message) {
	std::lock_guard<std::mutex> lock(mtx);
	data[currentIndex] = message;

	currentIndex++;
	if (currentIndex >= LogArraySize) {
		writeFlag.store(true);
		currentIndex = 0;
	}
}

void FD::LogWrite::file(const bool all) {

	//作ったはずのdirectoryがない
	if (!std::filesystem::is_directory(Internal::Log::FluidumLogFolderDirectory))
		throw std::runtime_error("Failed to find FluidumLog cirectory.");

	std::ofstream ofs(filePath, std::ios::app);//追記モード

	if (!ofs)
		throw std::runtime_error("Failed to write log data.");


	//出力
	static_assert(sizeof(std::string::value_type) == 1, "特殊な環境です．");
	using namespace Internal::Log;

	if (all) {
		std::lock_guard<std::mutex> lock(Internal::Log::Data::mtx);
		for (const auto& x : Data::data) {
			ofs.write(x.data(), x.size());
		}
	}
	else {
		std::lock_guard<std::mutex> lock(Internal::Log::Data::mtx);
		for (uint16_t i = 0; i < Data::currentIndex; i++) {
			ofs.write(Data::data[i].data(), Data::data[i].size());
		}
	}

}

void FD::LogWrite::write() {

	while (true) {
		//待機
		while (!Internal::Log::Data::writeFlag.load()) {
			const bool request = this->writeLogThread.get_stop_token().stop_requested();
			if (request)//終了要請されたら終わる
				break;
		}

		//通常->全てを出力
		if (!writeLogThread.get_stop_token().stop_requested()) {
			this->file();
		}
		//最後->currentIndexまで出力
		else {
			this->file(false);
			break;
		}

		Internal::Log::Data::writeFlag.store(false);
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

FD::LogWrite::LogWrite()
	: filePath(Internal::Log::makeFilePath())
{
	//ディレクトリがなければ作る
	if (!std::filesystem::is_directory(Internal::Log::FluidumLogFolderDirectory)) {
		if (!std::filesystem::create_directory(Internal::Log::FluidumLogFolderDirectory))
			throw std::runtime_error("Failed to create FluidumLog directory.");
	}

	this->writeLogThread = std::jthread(&LogWrite::write, this);
}


std::string FD::LogRead::getLatestMessage() const {
	std::lock_guard<std::mutex> lock(Internal::Log::Data::mtx);
	using namespace Internal::Log;
	return Data::data[Data::currentIndex == 0 ? LogArraySize - 1 : Data::currentIndex - 1];
}
