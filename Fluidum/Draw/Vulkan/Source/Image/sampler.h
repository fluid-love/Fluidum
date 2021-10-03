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
		explicit Sampler(ManagerPassKey, const Data::SamplerData& data, const Parameter& parameter);
		~Sampler() = default;
		FluidumUtils_Class_Default_CopyMove(Sampler)

	private:
		void create(const Data::SamplerData& data, const Parameter& parameter);
	public:
		_NODISCARD const Data::SamplerInfo& get() const noexcept;
		void destroy();

	private://data
		Data::SamplerInfo info = {};
	};



}
