#include "FVKshadermodule.h"

//前方宣言
std::vector<char> readFile(const char* filePath);

void FVK::Internal::ShaderModule::create(const Data::ShaderModuleData& data, const Parameter& parameter) {
	auto result = data.get<FvkType::LogicalDevice>().device.createShaderModule(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess)
		Exception::throwFailedToCreate("Failed to creare ShaderModule");

	this->info.shaderModule = result.value;

	this->info.device = data.get<FvkType::LogicalDevice>().device;
}

FVK::Internal::ShaderModule::ShaderModule(ManagerPassKey, const Data::ShaderModuleData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

const FVK::Internal::Data::ShaderModuleInfo& FVK::Internal::ShaderModule::get() const noexcept {
	assert(info.shaderModule);
	return this->info;
}

void FVK::Internal::ShaderModule::destroy() {
	assert(info.shaderModule);
	this->info.device.destroyShaderModule(info.shaderModule);
}

std::vector<char> FVK::Internal::ShaderModule::readFile(const char* filePath) {
	std::ifstream ifs(filePath, std::ios::ate | std::ios::binary);

	if (!ifs)
		throw std::runtime_error("Failed to open shader file");

	std::size_t fileSize = static_cast<std::size_t>(ifs.tellg());
	std::vector<char> buffer(fileSize);

	ifs.seekg(0);
	ifs.read(buffer.data(), fileSize);

	return buffer;
}