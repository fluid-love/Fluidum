#include "FVKqueue.h"

FVK::Internal::Queue::Queue(ManagerPassKey, const Data::QueueData& data, const Parameter& parameter) {
	this->getQueue(data,parameter);
}

void FVK::Internal::Queue::getQueue(const Data::QueueData& data, const Parameter& parameter) {
	
	const auto result = data.get<FvkType::LogicalDevice>().device.getQueue(
		parameter.pInfo->queueFamilyIndex,
		parameter.pInfo->queueIndex
	);

	this->info.queue = result;
	
}

void FVK::Internal::Queue::destroy() const noexcept {

}

const FVK::Internal::Data::QueueInfo& FVK::Internal::Queue::get() const noexcept {
	return this->info;
}