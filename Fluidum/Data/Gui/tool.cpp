#include "tool.h"

void FD::ToolBarWrite::lock(const FU::Class::ClassCode::CodeType code) {
	using namespace TopBar::Internal;
	const auto itr = std::find_if(Data::funcs.begin(), Data::funcs.end(), [&](auto& x) {
		return x.code == code;
		}
	);
	itr->select = true;
	assert(itr != Data::funcs.end());
	const UIF16 distance = static_cast<UIF16>(std::distance(Data::funcs.begin(), itr));
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

const std::vector<FD::UIF16>* FD::ToolBarRead::getIndices() const noexcept {
	using namespace ::FD::TopBar::Internal;
	return &Data::indices;
}

void FD::ToolBarRead::call(const UIF16 index) const {
	using namespace ::FD::TopBar::Internal;
	Data::funcs.at(index).func();
}


