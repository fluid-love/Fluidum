#include "topbar.h"

const std::vector<FD::TopBar::Info>* FD::TopBarRead::getInfo() const noexcept {
	using namespace ::FD::TopBar::Internal;
	return &Data::funcs;
}

const std::vector<uint16_t>* FD::TopBarRead::getIndices() const noexcept {
	using namespace ::FD::TopBar::Internal;
	return &Data::indices;
}

void FD::TopBarRead::call(const uint16_t index) const {
	using namespace ::FD::TopBar::Internal;
	Data::funcs.at(index).func();
}