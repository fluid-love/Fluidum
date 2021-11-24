#include "array.h"
#include "../../Limits/limits.h"

FD::Calc::Array::Key FD::Calc::ArrayWrite::generateKey(const Array::ValueType type) {
	using namespace Array::Internal;
	using enum Array::ValueType;
	const std::optional<Array::Index> index = this->garbage.findEmpty(type);
	if (type == Number) {
		if (!index) {
			if (Data::numbers.size() >= Array::Limits::MaxSize)
				throw Exception::MaxSize;
			Data::numbers.emplace_back();
			return static_cast<Array::Key>(Data::numbers.size() - 1);
		}
		else {
			assert(Data::numbers.empty());
			garbage.erase(type, index.value());
			return index.value();
		}
	}
	else if (type == String) {
		if (!index) {
			if (Data::strings.size() >= Array::Limits::MaxSize)
				throw Exception::MaxSize;
			Data::strings.emplace_back();
			return static_cast<Array::Key>(Data::strings.size() - 1);
		}
		else {
			assert(Data::strings.empty());
			garbage.erase(type, index.value());
			return index.value();
		}
	}
	else {
		assert(type == Bit);
		if (!index) {
			if (Data::bits.size() >= Array::Limits::MaxSize)
				throw Exception::MaxSize;
			Data::bits.emplace_back();
			return static_cast<Array::Key>(Data::bits.size() - 1);
		}
		else {
			assert(Data::bits.empty());
			garbage.erase(type, index.value());
			return index.value();
		}
	}
}

std::optional<FD::Calc::Array::Index> FD::Calc::ArrayWrite::GarbageManager::findEmpty(const Array::ValueType type) const {
	using enum Array::ValueType;

	auto itr = std::find_if(this->garbage.begin(), this->garbage.end(), [=](auto& x) {return x.first == type; });
	if (itr == this->garbage.end())
		return std::nullopt;

	return itr->second;
}

void FD::Calc::ArrayWrite::GarbageManager::reuse(const Array::ValueType type, const Array::Index index) {
	garbage.emplace_back(std::make_pair(type, index));
}

FD::Calc::Array::Key FD::Calc::ArrayWrite::make(const Array::ValueType type) {
	std::lock_guard<std::mutex> lock(Array::Internal::Data::mtx);

	return this->generateKey(type);
}

void FD::Calc::ArrayWrite::reset() {
	using namespace Array::Internal;
	using enum Array::ValueType;

	std::lock_guard<std::mutex> lock(Data::mtx);

	Data::numbers.clear();
	Data::numbers.shrink_to_fit();
	Data::strings.clear();
	Data::strings.shrink_to_fit();
	Data::bits.clear();
	Data::bits.shrink_to_fit();

	this->garbage.reset();
}

void FD::Calc::ArrayWrite::GarbageManager::erase(const Array::ValueType type, const Array::Index index) {
	auto itr = std::find_if(this->garbage.begin(), this->garbage.end(), [=](auto& x) {return (x.first == type) && (x.second == index); });

	if (itr != garbage.end())
		garbage.erase(itr);
}

void FD::Calc::ArrayWrite::GarbageManager::reset() {
	garbage.clear();
	garbage.shrink_to_fit();
}