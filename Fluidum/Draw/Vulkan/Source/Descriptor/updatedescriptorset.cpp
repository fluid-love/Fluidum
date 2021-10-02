#include "FVKupdatedescriptorset.h"

void FVK::Internal::UpdateDescriptorSet::updateUniformBuffer(Parameter& parameter) {

	assert(device && descriptorSet && buffer);

	vk::DescriptorBufferInfo bufferInfo = {
		.buffer = this->buffer,
		.offset = parameter.pInfo->pUniformBufferInfo->offset,
		.range = parameter.pInfo->pUniformBufferInfo->range
	};

	parameter.pInfo->pBufferInfo = &bufferInfo;
	parameter.pInfo->dstSet = this->descriptorSet;

	device.updateDescriptorSets(*parameter.pInfo, 0);

}