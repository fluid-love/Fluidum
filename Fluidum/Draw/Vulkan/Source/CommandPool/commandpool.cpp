#include "commandpool.h"

FVK::Internal::CommandPool::CommandPool(ManagerPassKey, const Data::CommandPoolData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::CommandPool::create(const Data::CommandPoolData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createCommandPool(*parameter.pInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create CommandPool({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.commandPool = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.commandPool = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::CommandPoolInfo& FVK::Internal::CommandPool::get() const noexcept {
	assert(this->info.commandPool);
	return this->info;
}

void FVK::Internal::CommandPool::destroy() noexcept {
	assert(this->info.commandPool);
	this->info.device.destroyCommandPool(this->info.commandPool);
}
