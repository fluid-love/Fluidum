#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class Fence final {
	public:

		struct CreateInfo : public vk::FenceCreateInfo {
		public:
			using vk::FenceCreateInfo::flags;
		private:
			using vk::FenceCreateInfo::sType;
			using vk::FenceCreateInfo::pNext;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

	public:
		explicit Fence(ManagerPassKey, const Data::FenceData& data, const Parameter& parameter);
		~Fence() = default;
		FluidumUtils_Class_Default_CopyMove(Fence)

	private:
		void create(const Data::FenceData& data, const Parameter& parameter);
	public:
		_NODISCARD const Data::FenceInfo& get() const noexcept;
		void destroy();

	private:
		Data::FenceInfo info = {};

	};




}
