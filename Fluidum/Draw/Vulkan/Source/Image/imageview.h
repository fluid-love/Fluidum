#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class ImageView final {
	public:
		struct CreateInfo : public vk::ImageViewCreateInfo {
		public:
			using vk::ImageViewCreateInfo::flags;
			std::optional<Key::ImageVariantKey> imageKey = std::nullopt;
			using vk::ImageViewCreateInfo::viewType;
			using vk::ImageViewCreateInfo::format;
			using vk::ImageViewCreateInfo::components;
			using vk::ImageViewCreateInfo::subresourceRange;
		private:
			using vk::ImageViewCreateInfo::sType;
			using vk::ImageViewCreateInfo::pNext;
			using vk::ImageViewCreateInfo::image;
		private:
			friend ImageView;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

		struct SwapchainParameter {
			uint32_t imageIndex = 0;//swapchainÇ≈çÏê¨ÇµÇΩcountà»â∫Ç≈Ç†ÇÈïKóvÇ™Ç†ÇÈ
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::SwapchainVariantKey> swapchainKey = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit ImageView(ManagerPassKey, const Data::ImageViewData& data, Parameter& parameter);

		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong		
		explicit ImageView(ManagerPassKey, const Data::ImageViewSwapchainData& data, const SwapchainParameter& parameter);

		~ImageView() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(ImageView)

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::ImageViewData& data, Parameter& parameter);

		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		void create(const Data::ImageViewSwapchainData& data, const SwapchainParameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		const Data::ImageViewInfo& get() const noexcept;

	private:
		Data::ImageViewInfo info{};

	};

}
