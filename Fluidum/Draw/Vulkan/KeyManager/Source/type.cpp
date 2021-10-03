#include "type.h"

FKM::OrderKey::OrderKey(const Type val) noexcept :index(val) {}

FKM::OrderKey::operator Type() const noexcept {
	return this->index;
}

void FKM::OrderKey::operator=(const Type val) noexcept {
	this->index = val;
}

bool FKM::OrderKey::operator==(const OrderKey key) const noexcept {
	return this->index == key.index;
}

void FKM::OrderKey::operator++(int) noexcept {
	assert(index < std::numeric_limits<Type>::max());
	index++;
}

bool FKM::OrderKey::operator==(Type val) const noexcept {
	return this->index == val;
}

////////////////////////////////////////////////////////////////////////////////////////

FKM::IndexKey::operator Type() const noexcept {
	return this->order;
}

void FKM::IndexKey::operator=(const Type val) noexcept {
	this->order = val;
}

bool FKM::IndexKey::operator==(const IndexKey key) const noexcept {
	return this->order == key.order;
}

bool FKM::IndexKey::operator!=(const IndexKey key) const noexcept {
	return this->order != key.order;
}

bool FKM::IndexKey::operator==(const Type val) const noexcept {
	return this->order == val;
}

FKM::IndexKey::IndexKey(const Type val) noexcept :order(val) {}

