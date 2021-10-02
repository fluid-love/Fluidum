#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class ShaderModule final {
	public:

		struct CreateInfo : public vk::ShaderModuleCreateInfo {
		public:
			using vk::ShaderModuleCreateInfo::flags;
			using vk::ShaderModuleCreateInfo::codeSize;
			using vk::ShaderModuleCreateInfo::pCode;
		private:
			using vk::ShaderModuleCreateInfo::sType;
			using vk::ShaderModuleCreateInfo::pNext;
		};

		struct Parameter {
			CreateInfo* pInfo;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit ShaderModule(ManagerPassKey, const Data::ShaderModuleData& data, const Parameter& parameter);
		~ShaderModule() = default;
		FluidumUtils_Class_Default_CopyMove(ShaderModule)

	private:
		void create(const Data::ShaderModuleData& data, const Parameter& parameter);
	public:
		const Data::ShaderModuleInfo& get() const noexcept;
		void destroy();

	public:
		_NODISCARD static std::vector<char> readFile(const char* filePath);

	private:
		Data::ShaderModuleInfo info = {};
	};

}
