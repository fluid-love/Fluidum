#pragma once

#include "../Common/FVKinclude.h"

namespace FVK::Internal {

	class UniformBuffer final {
	public:
		struct Parameter final {
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
		};

		struct Object final {
			alignas(16) glm::mat4 model;
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};

	public:
		explicit UniformBuffer(ManagerPassKey, const Data::UniformBufferData& data, const Parameter& parameter);
		~UniformBuffer() = default;
		FluidumUtils_Class_Default_CopyMove(UniformBuffer)

	private:
		void create(const Data::UniformBufferData& data, const Parameter& parameter);
	public:
		const Data::UniformBufferInfo& get() const noexcept;
		void destroy();

	private:
		Data::UniformBufferInfo info = {};
	};


}

