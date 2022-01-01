#pragma once

#include "../Common/include.h"

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
		/*
		Exception:
			NotSupported
		*/
		//strong
		explicit UniformBuffer(ManagerPassKey, const Data::UniformBufferData& data, const Parameter& parameter);
		
		~UniformBuffer() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(UniformBuffer)

	private:
		/*
		Exception:
			NotSupported
		*/
		//strong
		void create(const Data::UniformBufferData& data, const Parameter& parameter);
	
	public:
		//no-throw
		void destroy() noexcept;
	
	public:
		//no-throw
		[[nodiscard]] const Data::UniformBufferInfo& get() const noexcept;

	private:
		Data::UniformBufferInfo info{};

	};

}

