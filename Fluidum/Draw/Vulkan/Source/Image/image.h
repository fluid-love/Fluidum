#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Image final {
	public:
		using CreateInfo = vk::ImageCreateInfo;

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
		explicit Image(ManagerPassKey, const Data::ImageData& data, const Parameter& parameter);

		~Image() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(Image);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::ImageData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::ImageInfo& get() const noexcept;

	private:
		Data::ImageInfo info{};

	};

}
