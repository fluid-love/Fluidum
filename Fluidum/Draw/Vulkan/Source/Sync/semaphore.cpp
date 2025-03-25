#include "semaphore.h"

FVK::Internal::Semaphore::Semaphore(ManagerPassKey, const Data::SemaphoreData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Semaphore::create(const Data::SemaphoreData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createSemaphore(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Semaphore({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	this->info.semaphore = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.semaphore = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::SemaphoreInfo& FVK::Internal::Semaphore::get() const noexcept {
	assert(this->info.semaphore);
	return this->info;
}

void FVK::Internal::Semaphore::destroy() noexcept {
	assert(this->info.semaphore);
	this->info.device.destroySemaphore(this->info.semaphore);
}


