#include "type.h"

Fluidum::KeyManager::OrderKey::OrderKey(const Type val) noexcept :index(val) {}

Fluidum::KeyManager::OrderKey::operator Type() const noexcept {
	return this->index;
}

void Fluidum::KeyManager::OrderKey::operator=(const Type val) noexcept {
	this->index = val;
}

bool Fluidum::KeyManager::OrderKey::operator==(const OrderKey key) const noexcept {
	return this->index == key.index;
}

void Fluidum::KeyManager::OrderKey::operator++(int) noexcept {
	assert(index < std::numeric_limits<Type>::max());
	index++;
}

bool Fluidum::KeyManager::OrderKey::operator==(Type val) const noexcept {
	return this->index == val;
}

////////////////////////////////////////////////////////////////////////////////////////

Fluidum::KeyManager::IndexKey::operator Type() const noexcept {
	return this->order;
}

void Fluidum::KeyManager::IndexKey::operator=(const Type val) noexcept {
	this->order = val;
}

bool Fluidum::KeyManager::IndexKey::operator==(const IndexKey key) const noexcept {
	return this->order == key.order;
}

bool Fluidum::KeyManager::IndexKey::operator!=(const IndexKey key) const noexcept {
	return this->order != key.order;
}

bool Fluidum::KeyManager::IndexKey::operator==(const Type val) const noexcept {
	return this->order == val;
}

Fluidum::KeyManager::IndexKey::IndexKey(const Type val) noexcept :order(val) {}

