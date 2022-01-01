#pragma once

#include "../Common/include.h"

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
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit ShaderModule(ManagerPassKey, const Data::ShaderModuleData& data, const Parameter& parameter);
		
		~ShaderModule() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(ShaderModule)

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::ShaderModuleData& data, const Parameter& parameter);
	
	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::ShaderModuleInfo& get() const noexcept;

	public:
		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] static std::vector<char> readFile(const char* filePath);

	private:
		Data::ShaderModuleInfo info{};

	};

}
