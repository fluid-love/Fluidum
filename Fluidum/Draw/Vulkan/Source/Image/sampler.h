#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Sampler final {
	public:
		struct CreateInfo : public vk::SamplerCreateInfo {
		public:
			using vk::SamplerCreateInfo::flags;
			using vk::SamplerCreateInfo::magFilter;
			using vk::SamplerCreateInfo::minFilter;
			using vk::SamplerCreateInfo::mipmapMode;
			using vk::SamplerCreateInfo::addressModeU;
			using vk::SamplerCreateInfo::addressModeV;
			using vk::SamplerCreateInfo::addressModeW;
			using vk::SamplerCreateInfo::mipLodBias;
			using vk::SamplerCreateInfo::anisotropyEnable;
			using vk::SamplerCreateInfo::maxAnisotropy;
			using vk::SamplerCreateInfo::compareEnable;
			using vk::SamplerCreateInfo::compareOp;
			using vk::SamplerCreateInfo::minLod;
			using vk::SamplerCreateInfo::maxLod;
			using vk::SamplerCreateInfo::borderColor;
			using vk::SamplerCreateInfo::unnormalizedCoordinates;
		private:
			using vk::SamplerCreateInfo::sType;
			using vk::SamplerCreateInfo::pNext;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit Sampler(ManagerPassKey, const Data::SamplerData& data, const Parameter& parameter);

		~Sampler() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Sampler)

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::SamplerData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;
		
	public:
		//no-throw
		[[nodiscard]] const Data::SamplerInfo& get() const noexcept;
		
	private:
		Data::SamplerInfo info{};

	};

}
