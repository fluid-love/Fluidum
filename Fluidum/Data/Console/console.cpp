#include "console.h"

void FD::ConsoleWrite::busy(const bool val) const {
	std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);
	using namespace Console::Internal;

	Data::busy = true;
}

void FD::ConsoleWrite::push_input(const std::string& message) const {
	std::string msg = ">" + message;
	this->push(std::move(msg));
}

void FD::ConsoleWrite::push_input(std::string&& message) const {
	message.insert(message.begin(),'>');
	this->push(std::move(message));
}

std::pair<bool, FD::Console::Info> FD::ConsoleRead::get(const std::size_t index) const {
	std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);
	using namespace Console::Internal;
	if (index >= Data::texts.size())
		return { false, {} };

	return { true,Data::texts[index] };
}

bool FD::ConsoleRead::busy() const {
	std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);
	using namespace Console::Internal;

	return Data::busy;
}

std::size_t FD::ConsoleRead::size() const {
	std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);
	using namespace Console::Internal;

	return Data::texts.size();
}
