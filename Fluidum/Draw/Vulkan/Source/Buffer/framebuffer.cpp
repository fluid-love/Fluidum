#include "FVKframebuffer.h"

FVK::Internal::FrameBuffer::FrameBuffer(ManagerPassKey, const Data::FrameBufferData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::FrameBuffer::create(const Data::FrameBufferData& data, Parameter& parameter) {

	parameter.pInfo->renderPass = data.get<FvkType::RenderPass>().renderPass;

	const auto& imageViewRefs = data.get<FvkType::ImageView_Vector>();
	std::vector<vk::ImageView> imageViews(imageViewRefs.size());
	for (std::size_t i = 0, size = imageViewRefs.size(); i < size; i++) {
		imageViews[i] = imageViewRefs[i].get().imageView;
	}

	parameter.pInfo->pAttachments = imageViews.data();
	parameter.pInfo->attachmentCount = static_cast<uint32_t>(imageViews.size());

	auto result = data.get<FvkType::LogicalDevice>().device.createFramebuffer(*parameter.pInfo, nullptr);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to create FrameBuffer");

	this->info.frameBuffer = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}
//
//void FVK::Internal::FrameBuffer::create(const Data::OffScreenFrameBufferData& data, Parameter& parameter) {
//
//	auto result = data.get<FvkType::LogicalDevice>().device.createFramebuffer(*parameter.pInfo, nullptr);
//
//	if (result.result != vk::Result::eSuccess)
//		Exception::throwFailedToCreate("Failed to create FrameBuffer");
//
//	this->info.frameBuffer = result.value;
//}


const FVK::Internal::Data::FrameBufferInfo& FVK::Internal::FrameBuffer::get() const noexcept {
	assert(this->info.frameBuffer);
	return this->info;
}

void FVK::Internal::FrameBuffer::destroy() {
	assert(this->info.frameBuffer);
	this->info.device.destroyFramebuffer(this->info.frameBuffer);
}
