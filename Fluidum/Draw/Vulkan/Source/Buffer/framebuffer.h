#pragma once

#include "../Common/FVKinclude.h"

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
		explicit FrameBuffer(ManagerPassKey, const Data::FrameBufferData& data, Parameter& parameter);
		~FrameBuffer() = default;
		FluidumUtils_Class_Default_CopyMove(FrameBuffer)

	private:
		void create(const Data::FrameBufferData& data, Parameter& parameter);
	public:
		const Data::FrameBufferInfo& get() const noexcept;
		void destroy();

	private:
		Data::FrameBufferInfo info = {};
	};
}
