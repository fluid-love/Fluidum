#include "imageview.h"

FVK::Internal::ImageView::ImageView(ManagerPassKey, const Data::ImageViewData& data, Parameter& parameter) {
	this->create(data, parameter);
}

FVK::Internal::ImageView::ImageView(ManagerPassKey, const Data::ImageViewSwapchainData& data, const SwapchainParameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::ImageView::create(const Data::ImageViewData& data, Parameter& parameter) {

	parameter.pInfo->image = data.get<FvkType::Image>().image;

	auto result = data.get<FvkType::LogicalDevice>().device.createImageView(*parameter.pInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create ImageView({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.imageView = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.imageView = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

void FVK::Internal::ImageView::create(const Data::ImageViewSwapchainData& data, const SwapchainParameter& parameter) {
	std::vector<vk::Image> images{};
	{
		auto result = data.get<FvkType::LogicalDevice>().device.getSwapchainImagesKHR(data.get<FvkType::Swapchain>().swapchain);
		if (result.result != vk::Result::eSuccess) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to get SwapchainImages({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}
		images = std::move(result.value);
	}

	vk::ImageSubresourceRange range{
		range.aspectMask = vk::ImageAspectFlagBits::eColor,
		range.baseMipLevel = 0,
		range.levelCount = 1,
		range.baseArrayLayer = 0,
		range.layerCount = 1,
	};

	//out of range
	if (images.size() <= parameter.imageIndex)
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "images.size() <= parameter.imageIndex");


	vk::ImageViewCreateInfo viewInfo{
		.image = images.at(parameter.imageIndex),
		.viewType = vk::ImageViewType::e2D,
		.format = data.get<FvkType::Swapchain>().format,
		.subresourceRange = range
	};
	
	auto result = data.get<FvkType::LogicalDevice>().device.createImageView(viewInfo);
	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create ImageView({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.imageView = result.value;
	static_assert(noexcept(info.imageView = result.value));

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::ImageViewInfo& FVK::Internal::ImageView::get() const noexcept {
	assert(this->info.imageView);
	return this->info;
}

void FVK::Internal::ImageView::destroy() noexcept {
	assert(this->info.imageView);
	this->info.device.destroyImageView(this->info.imageView);
}


