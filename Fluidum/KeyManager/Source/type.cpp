#include "type.h"

FKM::OrderKey::OrderKey(const KeyType val) noexcept :index(val) {}

FKM::OrderKey::operator KeyType() const noexcept {
	return this->index;
}

void FKM::OrderKey::operator=(const KeyType val) noexcept {
	this->index = val;
}

bool FKM::OrderKey::operator==(const OrderKey key) const noexcept {
	return this->index == key.index;
}

void FKM::OrderKey::operator++(int) noexcept {
	assert(index < std::numeric_limits<KeyType>::max());
	index++;
}

bool FKM::OrderKey::operator==(KeyType val) const noexcept {
	return this->index == val;
}

////////////////////////////////////////////////////////////////////////////////////////

FKM::IndexKey::operator KeyType() const noexcept {
	return this->order;
}

void FKM::IndexKey::operator=(const KeyType val) noexcept {
	this->order = val;
}

bool FKM::IndexKey::operator==(const IndexKey key) const noexcept {
	return this->order == key.order;
}

bool FKM::IndexKey::operator!=(const IndexKey key) const noexcept {
	return this->order != key.order;
}

bool FKM::IndexKey::operator==(const KeyType val) const noexcept {
	return this->order == val;
}

FKM::IndexKey::IndexKey(const KeyType val) noexcept : order(val) {}

