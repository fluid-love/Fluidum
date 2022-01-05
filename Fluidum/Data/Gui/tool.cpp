#include "tool.h"

void FD::ToolBarWrite::lock(const FU::Class::ClassCode::CodeType code) {
	using namespace TopBar::Internal;
	const auto itr = std::find_if(Data::funcs.begin(), Data::funcs.end(), [&](auto& x) {
		return x.code == code;
		}
	);
	itr->select = true;
	assert(itr != Data::funcs.end());
	const auto distance = static_cast<decltype(Data::indices)::size_type>(std::distance(Data::funcs.begin(), itr));
	Data::indices.emplace_back(distance);
}

void FD::ToolBarWrite::unlock(const FU::Class::ClassCode::CodeType code) {
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

const std::vector<FD::TopBar::Info>* FD::ToolBarRead::getInfo() const noexcept {
	using namespace ::FD::TopBar::Internal;
	return &Data::funcs;
}

const std::vector<uint16_t>* FD::ToolBarRead::getIndices() const noexcept {
	using namespace ::FD::TopBar::Internal;
	return &Data::indices;
}

void FD::ToolBarRead::call(const uint16_t index) const {
	using namespace ::FD::TopBar::Internal;
	Data::funcs.at(index).func();
}


