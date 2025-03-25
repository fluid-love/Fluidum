#include "queue.h"

FVK::Internal::Queue::Queue(ManagerPassKey, const Data::QueueData& data, const Parameter& parameter) noexcept {
	//no-throw
	this->getQueue(data, parameter);
}

void FVK::Internal::Queue::getQueue(const Data::QueueData& data, const Parameter& parameter) noexcept {

	//no-throw
	const vk::Device device = data.get<FvkType::LogicalDevice>().device;
	static_assert(std::is_nothrow_constructible_v<vk::Device, const vk::Device&>);

	assert(parameter.pInfo);

	//no-throw
	const vk::Queue result = device.getQueue(
		parameter.pInfo->queueFamilyIndex,
		parameter.pInfo->queueIndex
	);
	static_assert(std::is_nothrow_constructible_v<vk::Queue, vk::Queue&&>);

	this->info.queue = result;
	static_assert(noexcept(this->info.queue = result));
}

void FVK::Internal::Queue::destroy() const noexcept {
	//There is nothing to destroy, but the function is there for the sake of consistency.
}

const FVK::Internal::Data::QueueInfo& FVK::Internal::Queue::get() const noexcept {
	assert(this->info.queue);
	return this->info;
}