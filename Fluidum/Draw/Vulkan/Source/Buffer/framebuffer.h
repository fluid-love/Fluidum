#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class FrameBuffer final {
	public:
		struct CreateInfo : public vk::FramebufferCreateInfo {
		public:
			using vk::FramebufferCreateInfo::flags;
			std::optional<Key::RenderPassVariantKey> renderPassKey = std::nullopt;
			std::optional<Key::ImageViewVariantVectorKey> imageViewKeys = std::nullopt;
			using vk::FramebufferCreateInfo::width;
			using vk::FramebufferCreateInfo::height;
			using vk::FramebufferCreateInfo::layers;
		private:
			using vk::FramebufferCreateInfo::sType;
			using vk::FramebufferCreateInfo::pNext;
			using vk::FramebufferCreateInfo::renderPass;
			using vk::FramebufferCreateInfo::attachmentCount;
			using vk::FramebufferCreateInfo::pAttachments;
		private:
			friend FrameBuffer;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		explicit FrameBuffer(ManagerPassKey, const Data::FrameBufferData& data, Parameter& parameter);

		~FrameBuffer() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(FrameBuffer)

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		void create(const Data::FrameBufferData& data, Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::FrameBufferInfo& get() const noexcept;

	private:
		Data::FrameBufferInfo info{};

	};

}
