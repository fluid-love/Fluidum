#include "info.h"

void FKM::Internal::OrderCounter::operator++(int) {
	constexpr OrderKey::Type max = std::numeric_limits<OrderKey::Type>::max();
	if (counter == max)
		Exception::Internal::throwReachIndexLimit();
	this->counter++;
}

FKM::Internal::OrderCounter::operator FKM::OrderKey() const noexcept {
	return this->counter;
}