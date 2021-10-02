#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class Image final {
	public:
		using CreateInfo = vk::ImageCreateInfo;

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit Image(ManagerPassKey,const Data::ImageData& data, const Parameter& parameter);
		~Image() = default;
		FluidumUtils_Class_Default_CopyMove(Image)

	private:
		void create(const Data::ImageData& data, const Parameter& parameter);
	public:
		const Data::ImageInfo& get() const noexcept;
		void destroy();

	private:
		Data::ImageInfo info = {};

	};
}
