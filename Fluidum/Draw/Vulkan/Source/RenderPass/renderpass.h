#pragma once

#include "../Common/include.h"

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
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit RenderPass(ManagerPassKey, const Data::RenderPassData& data, const Parameter& parameter);

		~RenderPass() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(RenderPass);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::RenderPassData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::RenderPassInfo& get() const noexcept;

	private:
		Data::RenderPassInfo info{};

	};

}
