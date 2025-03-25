#include "image.h"

FVK::Internal::ImGuiImage::ImGuiImage(ManagerPassKey, const Data::ImGuiImageData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::ImGuiImage::create(const Data::ImGuiImageData& data, const Parameter& parameter) {

	auto* backend = ImGui_ImplVulkan_BackendData();
	vk::DescriptorSetLayout layout = backend->DescriptorSetLayout;
	{
		vk::DescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.descriptorPool = data.get<FvkType::ImGui>().descriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &layout;

		auto result = data.get<FvkType::ImGui>().device.allocateDescriptorSets(allocateInfo);
		if (result.result != vk::Result::eSuccess || result.value.empty()) {
			GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to allocate DescriptorSets({}).", vk::to_string(result.result));
			Exception::throwFailedToCreate();
		}

		//no-throw
		this->info.descriptorSet = result.value[0].operator VkDescriptorSet();
		static_assert(noexcept(info.descriptorSet = result.value[0].operator VkDescriptorSet()));
	}
	{
		vk::DescriptorImageInfo imageInfo[1] = {};
		imageInfo[0].sampler = data.get<FvkType::Sampler>().sampler;
		imageInfo[0].imageView = data.get<FvkType::Texture>().imageView;
		imageInfo[0].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		vk::WriteDescriptorSet write[1] = {};
		write[0].dstSet = this->info.descriptorSet;
		write[0].descriptorCount = 1;
		write[0].descriptorType = vk::DescriptorType::eCombinedImageSampler;
		write[0].pImageInfo = imageInfo;

		//no-throw
		data.get<FvkType::ImGui>().device.updateDescriptorSets(1, write, 0, nullptr);
	}

	//no-throw
	this->info.device = data.get<FvkType::ImGui>().device;
	this->info.descriptorPool = data.get<FvkType::ImGui>().descriptorPool;

	static_assert(noexcept(info.device = data.get<FvkType::ImGui>().device));
	static_assert(noexcept(info.descriptorPool = data.get<FvkType::ImGui>().descriptorPool));

}

const FVK::Internal::Data::ImGuiImageInfo& FVK::Internal::ImGuiImage::get() const noexcept {
	assert(info.descriptorSet);
	return this->info;
}

void FVK::Internal::ImGuiImage::destroy() {
	assert(info.descriptorSet && info.device);
	try {
		auto result = this->info.device.freeDescriptorSets(this->info.descriptorPool, vk::DescriptorSet(this->info.descriptorSet));
		if (result != vk::Result::eSuccess) {
			Exception::throwFailedToDestroy();
		}
	}
	catch (...) {
		std::rethrow_exception(std::current_exception());
	}
}
