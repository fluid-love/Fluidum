#include "shadermodule.h"

//forward declaration
std::vector<char> readFile(const char* filePath);

FVK::Internal::ShaderModule::ShaderModule(ManagerPassKey, const Data::ShaderModuleData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

void FVK::Internal::ShaderModule::create(const Data::ShaderModuleData& data, const Parameter& parameter) {
	auto result = data.get<FvkType::LogicalDevice>().device.createShaderModule(*parameter.pInfo);

	if (result.result != vk::Result::eSuccess) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to create ShaderModule({}).", vk::to_string(result.result));
		Exception::throwFailedToCreate();
	}

	//no-throw
	this->info.shaderModule = result.value;
	static_assert(noexcept(info.shaderModule = result.value));

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	static_assert(noexcept(info.device = data.get<FvkType::LogicalDevice>().device));
}

const FVK::Internal::Data::ShaderModuleInfo& FVK::Internal::ShaderModule::get() const noexcept {
	assert(info.shaderModule);
	return this->info;
}

void FVK::Internal::ShaderModule::destroy() noexcept {
	assert(info.shaderModule);
	this->info.device.destroyShaderModule(info.shaderModule);
}

std::vector<char> FVK::Internal::ShaderModule::readFile(const char* filePath) {
	std::ifstream ifs(filePath, std::ios::ate | std::ios::binary);

	if (!ifs)
		throw std::runtime_error("Failed to open shader file");

	const std::streampos fileSize = ifs.tellg();
	std::vector<char> buffer(fileSize);

	ifs.seekg(0);
	ifs.read(buffer.data(), fileSize);

	return buffer;
}