#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class RenderPass final {
	public:
		struct CreateInfo final : public vk::RenderPassCreateInfo {
		public:
			using vk::RenderPassCreateInfo::flags;
			using vk::RenderPassCreateInfo::attachmentCount;
			using vk::RenderPassCreateInfo::pAttachments;
			using vk::RenderPassCreateInfo::subpassCount;
			using vk::RenderPassCreateInfo::pSubpasses;
			using vk::RenderPassCreateInfo::dependencyCount;
			using vk::RenderPassCreateInfo::pDependencies;
		private:
			using vk::RenderPassCreateInfo::sType;
			using vk::RenderPassCreateInfo::pNext;
		};

		struct Parameter {
			CreateInfo* pInfo;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit RenderPass(ManagerPassKey, const Data::RenderPassData& data, const Parameter& parameter);
		~RenderPass() = default;

	private:
		void create(const Data::RenderPassData& data, const Parameter& parameter);
	public:
		const Data::RenderPassInfo& get() const noexcept;
		void destroy();

	private:
		Data::RenderPassInfo info = {};

	};

}
