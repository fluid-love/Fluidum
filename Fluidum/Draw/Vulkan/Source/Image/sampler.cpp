#include "sampler.h"

FVK::Internal::Sampler::Sampler(ManagerPassKey, const Data::SamplerData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Sampler::create(const Data::SamplerData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createSampler(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create Sampler");

	this->info.sampler = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::SamplerInfo& FVK::Internal::Sampler::get() const noexcept {
	assert(this->info.sampler);
	return this->info;
}

void FVK::Internal::Sampler::destroy() {
	assert(this->info.sampler);
	this->info.device.destroySampler(this->info.sampler);
}


