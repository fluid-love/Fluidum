#include "console.h"

std::pair<bool, FD::Console::Info> FD::ConsoleRead::get(const std::size_t index) const {
	std::lock_guard<std::mutex> lock(Console::Internal::Data::mtx);
	using namespace Console::Internal;
	if (index >= Data::texts.size())
		return { false, {} };

	return { true,Data::texts[index] };
}
