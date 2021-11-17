#include "calc.h"

namespace FD {


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FD::CalcWrite::mode(const Calc::Mode val) const {
	using namespace Calc::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	Data::mode = val;
}

void FD::CalcWrite::setFirst(const bool val) const {
	using namespace Calc::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	Data::save = val;
}

void FD::CalcWrite::save() const noexcept {
	using namespace Calc::Internal;
	Data::save.store(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FD::Calc::Mode FD::CalcRead::mode() const {
	using namespace Calc::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return Data::mode;
}

bool FD::CalcRead::first() const {
	using namespace Calc::Internal;
	std::lock_guard<std::mutex> lock(Data::mtx);
	return Data::first;
}