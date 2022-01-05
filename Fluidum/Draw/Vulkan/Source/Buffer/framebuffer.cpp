#include "framebuffer.h"

FVK::Internal::FrameBuffer::FrameBuffer(ManagerPassKey, const Data::FrameBufferData& data, Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::FrameBuffer::create(const Data::FrameBufferData& data, Parameter& parameter) {

	parameter.pInfo->renderPass = data.get<FvkType::RenderPass>().renderPass;

	const auto& imageViewRefs = data.get<FvkType::ImageView_Vector>();
	std::vector<vk::ImageView> imageViews(imageViewRefs.size());
	for (Size i = 0, size = imageViewRefs.size(); i < size; i++) {
		imageViews[i] = imageViewRefs[i].get().imageView;
	}

	parameter.pInfo->pAttachments = imageViews.data();
	parameter.pInfo->attachmentCount = static_cast<UI32>(imageViews.size());

	auto result = data.get<FvkType::LogicalDevice>().device.createFramebuffer(*parameter.pInfo, nullptr);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create FrameBuffer({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.frameBuffer = result.value;
	this->info.device = data.get<FvkType::LogicalDevice>().device;

	static_assert(noexcept(info.frameBuffer = result.value));
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));

}

const FVK::Internal::Data::FrameBufferInfo& FVK::Internal::FrameBuffer::get() const noexcept {
	assert(this->info.frameBuffer);
	return this->info;
}

void FVK::Internal::FrameBuffer::destroy() noexcept {
	assert(this->info.frameBuffer);
	this->info.device.destroyFramebuffer(this->info.frameBuffer);
}
