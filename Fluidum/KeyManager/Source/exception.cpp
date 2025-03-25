#include "exception.h"

FKM::Exception::ErrorType FKM::Exception::Error::code() const noexcept {
	return this->val;
}

void FKM::Exception::Internal::throwCollisionOfKeys() {
	throw Error(ErrorType::CollisionOfKeys);
}

void FKM::Exception::Internal::throwReachIndexLimit() {
	throw Error(ErrorType::ReachIndexLimit);
}

void FKM::Exception::Internal::throwNotFound() {
	throw Error(ErrorType::NotFound);
}

void FKM::Exception::Internal::throwFailedToRemove() {
	throw Error(ErrorType::FailedToRemove);
}











