#include "FVKimage.h"
#include "..\ImGui\FVKimage.h"

FVK::Internal::Image::Image(ManagerPassKey, const Data::ImageData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::Image::create(const Data::ImageData& data, const Parameter& parameter) {


	auto result = data.get<FvkType::LogicalDevice>().device.createImage(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create Image");

	this->info.image = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

const FVK::Internal::Data::ImageInfo& FVK::Internal::Image::get() const noexcept {
	assert(this->info.image);
	return this->info;
}

void FVK::Internal::Image::destroy() {
	assert(this->info.image);
	this->info.device.destroyImage(this->info.image);
}
