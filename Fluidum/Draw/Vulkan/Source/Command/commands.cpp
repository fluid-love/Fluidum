#include "commands.h"

void FVK::Internal::Command::FunctionIndex::operator++(int) noexcept {
	this->index++;
}

FVK::Internal::Command::FunctionIndex::operator FVK::UIF32() const noexcept {
	return this->index;
}

void FVK::Internal::Command::FunctionIndex::operator=(const UIF32 val) noexcept {
	this->index = val;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FVK::Internal::Command::Commands::call() {
	{
		LockGuard lock(GMutex);
		internalCall();
	}
	condition.notify_one();
}

void FVK::Internal::Command::Commands::internalCall() {
	const auto currentSize = Blocks::size();

	//Start from the previous end index.
	const ConstIterator startItr = Blocks::cbegin() + index;

	//till CommandType::Next
	ConstIterator itr = std::find_if(
		startItr,
		Blocks::cend(),
		[&](const Block& x) {x.function(); index++; return x.type == CommandType::Next; }
	);

	//can't find Next at the end -> start from the beginning.
	if (itr == Blocks::cend()) {
		index = 0;
		itr = std::find_if(
			Blocks::cbegin(),
			Blocks::cend(),
			[&](const Block& x) {x.function(); index++; return x.type == CommandType::Next; }
		);
	}


	//	//once‚Ìˆ— 
	//	if (!this->onceIndices.empty()) {
	//		auto size = this->onceIndices.size();
	//		for (ui32 i = 0; i < size; i += 2) {
	//			if (onceIndices[i] == functionIndex) {//ŽÀs‚µ‚½‚Ì‚ÅÁ‚·
	//				auto temp = onceIndices[i + 1];
	//				adjustIndexMinus(VecType::cbegin() + onceIndices[i + 1]);

	//				VecType::erase(VecType::begin() + temp);
	//				onceIndices.erase(onceIndices.begin() + i, onceIndices.begin() + i + 2);
	//				i -= 2; size -= 2;
	//			}
	//		}
	//	}

}

