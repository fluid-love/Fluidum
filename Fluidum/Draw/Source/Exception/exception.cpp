#include "exception.h"

void FDR::Exception::Internal::throwFailed() {
	throw Error(ErrorType::Failed);
}