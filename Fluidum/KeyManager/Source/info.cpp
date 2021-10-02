#include "info.h"

void Fluidum::KeyManager::Internal::OrderCounter::operator++(int) {
	constexpr OrderKey::Type max = std::numeric_limits<OrderKey::Type>::max();
	if (counter == max)
		Exception::Internal::throwReachIndexLimit();
	this->counter++;
}

Fluidum::KeyManager::Internal::OrderCounter::operator Fluidum::KeyManager::OrderKey() const noexcept {
	return this->counter;
}