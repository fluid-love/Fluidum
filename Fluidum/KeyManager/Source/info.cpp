#include "info.h"

namespace FKM::Internal {
	constexpr OrderKey::KeyType Max = std::numeric_limits<OrderKey::KeyType>::max();
}

void FKM::Internal::OrderCounter::operator++(int) {
	if (count == Max)
		Exception::Internal::throwReachIndexLimit();
	this->count++;
}

FKM::OrderKey FKM::Internal::OrderCounter::next() const noexcept {
	return count + 1;
}

FKM::Internal::OrderCounter::operator FKM::OrderKey() const noexcept {
	return this->count;
}

FKM::OrderKey FKM::Internal::OrderCounter::current() const noexcept {
	return count;
}