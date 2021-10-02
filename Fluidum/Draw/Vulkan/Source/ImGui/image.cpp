#include "FVKimage.h"

FVK::Internal::ImGuiImage::ImGuiImage(ManagerPassKey, const Data::ImGuiImageData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::ImGuiImage::create(const Data::ImGuiImageData& data, const Parameter& parameter) {

	//vk::DescriptorSetLayoutBinding binding[1] = {};
	//binding[0].descriptorCount = 1;
	//binding[0].stageFlags = vk::ShaderStageFlagBits::eFragment;
	//binding[0].descriptorType = vk::DescriptorType::eCombinedImageSampler;

	//{
	//	vk::DescriptorSetLayoutCreateInfo createInfo = {};
	//	createInfo.bindingCount = 1;
	//	createInfo.pBindings = binding;

	//	auto result = data.get<FvkType::ImGui>().device.createDescriptorSetLayout(createInfo, nullptr);
	//	if (result.result != vk::Result::eSuccess)
	//		Exception::throwFailedToCreate("Failed to create DescriptorSetLayout");
	//	this->info.descriptorSetLayout = result.value;
	//}

	auto* backend = ImGui_ImplVulkan_BackendData();
	vk::DescriptorSetLayout layout = backend->DescriptorSetLayout;
	{
		vk::DescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.descriptorPool = data.get<FvkType::ImGui>().descriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &layout;

		auto result = data.get<FvkType::ImGui>().device.allocateDescriptorSets(allocateInfo);
		if (result.result != vk::Result::eSuccess)
			Exception::throwFailedToCreate("Failed to allocate DescriptorSet");
		this->info.descriptorSet = result.value.at(0).operator VkDescriptorSet();
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
		data.get<FvkType::ImGui>().device.updateDescriptorSets(1, write, 0, nullptr);
	}

	//ImGui_ImplVulkan_bindDescriptorSet(this->info.descriptorSet);

	this->info.device = data.get<FvkType::ImGui>().device;
	this->info.descriptorPool = data.get<FvkType::ImGui>().descriptorPool;
}

const FVK::Internal::Data::ImGuiImageInfo& FVK::Internal::ImGuiImage::get() const noexcept {
	assert(info.descriptorSet);
	return this->info;
}

void FVK::Internal::ImGuiImage::destroy() {
	assert(info.descriptorSet && info.device);
	auto result = this->info.device.freeDescriptorSets(this->info.descriptorPool, vk::DescriptorSet(this->info.descriptorSet));
	if (result != vk::Result::eSuccess)
		Exception::throwFailedToDestroy("Failed to free descriptorset");
}
