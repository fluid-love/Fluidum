#include "commands.h"

void FVK::Internal::Command::FunctionIndex::operator++(int) noexcept {
	this->index++;
}

FVK::Internal::Command::FunctionIndex::operator uint32_t() const noexcept {
	return this->index;
}

void FVK::Internal::Command::FunctionIndex::operator=(uint32_t val) noexcept {
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

	//前回の終了位置からスタート位置
	const ConstIterator startItr = Blocks::cbegin() + index;

	//CommandType::Nextまでループする
	ConstIterator itr = std::find_if(
		startItr,
		Blocks::cend(),
		[&](const Block& x) {x.function(); index++; return x.type == CommandType::Next; }
	);

	//最後まで行ってNextがない->はじめから探す
	if (itr == Blocks::cend()) {
		index = 0;
		itr = std::find_if(
			Blocks::cbegin(),
			Blocks::cend(),
			[&](const Block& x) {x.function(); index++; return x.type == CommandType::Next; }
		);
	}




	//	//onceの処理 
	//	if (!this->onceIndices.empty()) {
	//		auto size = this->onceIndices.size();
	//		for (ui32 i = 0; i < size; i += 2) {
	//			if (onceIndices[i] == functionIndex) {//実行したので消す
	//				auto temp = onceIndices[i + 1];
	//				adjustIndexMinus(VecType::cbegin() + onceIndices[i + 1]);

	//				VecType::erase(VecType::begin() + temp);
	//				onceIndices.erase(onceIndices.begin() + i, onceIndices.begin() + i + 2);
	//				i -= 2; size -= 2;
	//			}
	//		}
	//	}

}

