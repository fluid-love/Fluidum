#pragma once

#include "../Common/include.h"
#include "buffer.h"

namespace FVK::Internal {

	class IndexBuffer final {
	private:
		struct ParameterBase {
			ParameterBase() = delete;
			ParameterBase(void* ptr, const std::size_t& size, vk::IndexType indexType) : indices(ptr), size(size), indexType(indexType) {}

			void* indices = nullptr;
			std::size_t size = 0;
			vk::IndexType indexType = vk::IndexType::eUint32;
		};
	public:
		struct Parameter final :private ParameterBase {
			explicit Parameter(std::vector<uint32_t>* v) : ParameterBase(v->data(), sizeof(v->at(0))* v->size(), vk::IndexType::eUint32), bufferSize(sizeof(v->at(0))* v->size()) {}
			explicit Parameter(std::vector<uint16_t>* v) : ParameterBase(v->data(), sizeof(v->at(0))* v->size(), vk::IndexType::eUint16), bufferSize(sizeof(v->at(0))* v->size()) {}

		public:
			std::size_t bufferSize = 0;
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;

		private:
			friend IndexBuffer;
		};

	public:
		explicit IndexBuffer(ManagerPassKey, const Data::IndexBufferData& data, const Parameter& parameter);
		~IndexBuffer() = default;
		FluidumUtils_Class_Default_CopyMove(IndexBuffer)

	private:
		void create(const Data::IndexBufferData& data, const Parameter& parameter);
	public:
		const Data::IndexBufferInfo& get() const noexcept;
		void destroy();

	private:
		Data::IndexBufferInfo info = {};
	};
}
