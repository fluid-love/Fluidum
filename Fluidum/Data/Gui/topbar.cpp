#include "topbar.h"

void FD::TopBarWrite::lock(const FU::Class::ClassCode::CodeType code) {
	using namespace TopBar::Internal;
	const auto itr = std::find_if(Data::funcs.begin(), Data::funcs.end(), [&](auto& x) {
		return x.code == code;
		}
	);
	itr->select = true;
	assert(itr != Data::funcs.end());
	Data::indices.emplace_back(std::distance(Data::funcs.begin(), itr));
}

void FD::TopBarWrite::unlock(const FU::Class::ClassCode::CodeType code) {
	using namespace TopBar::Internal;
	auto itr = std::find_if(Data::funcs.begin(), Data::funcs.end(), [&](auto& x) {
		return x.code == code;
		}
	);
	itr->select = false;
	assert(itr != Data::funcs.end());
	auto erase = std::find(Data::indices.begin(), Data::indices.end(), std::distance(Data::funcs.begin(), itr));
	Data::indices.erase(erase);
}

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


