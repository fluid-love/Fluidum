#include "image.h"
#include "../ImGui/image.h"

FVK::Internal::Image::Image(ManagerPassKey, const Data::ImageData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Image::create(const Data::ImageData& data, const Parameter& parameter) {

	auto result = data.get<FvkType::LogicalDevice>().device.createImage(*parameter.pInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create Image({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.image = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(this->info.image = result.value));
	static_assert(noexcept(this->info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::ImageInfo& FVK::Internal::Image::get() const noexcept {
	assert(this->info.image);
	return this->info;
}

void FVK::Internal::Image::destroy() noexcept {
	assert(this->info.image);
	this->info.device.destroyImage(this->info.image);
}
