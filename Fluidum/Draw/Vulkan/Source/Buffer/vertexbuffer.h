#pragma once

#include "../Common/include.h"
#include "buffer.h"

namespace FVK::Internal {

	class VertexBuffer final {
	private:
		struct ParameterBase {
			ParameterBase() = delete;
			ParameterBase(void* ptr, std::size_t size) : vertices(ptr), size(size) {}

			std::size_t size;
			void* vertices = nullptr;
		};
	public:
		struct Parameter final :private ParameterBase {
			template<typename T>
			Parameter(std::vector<T>* v) : ParameterBase(v->data(), sizeof(v->at(0))* v->size()), bufferSize(sizeof(v->at(0))* v->size()) {}

		public:
			std::size_t bufferSize = 0;
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;

		private:
			friend VertexBuffer;
		};

	public:
		explicit VertexBuffer(ManagerPassKey, const Data::VertexBufferData& data,const Parameter& parameter);
		~VertexBuffer() = default;
		FluidumUtils_Class_Default_CopyMove(VertexBuffer)

	private:
		void create(const Data::VertexBufferData& data,const Parameter& parameter);
	public:
		const Data::VertexBufferInfo& get() const noexcept;
		void destroy();

	private:
		Data::VertexBufferInfo info = {};
	};


}
