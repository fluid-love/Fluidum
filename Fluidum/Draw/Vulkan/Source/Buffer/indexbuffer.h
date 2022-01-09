#pragma once

#include "../Common/include.h"
#include "buffer.h"

namespace FVK::Internal {

	class IndexBuffer final {
	private:
		struct ParameterBase {
			ParameterBase() = delete;
			ParameterBase(void* ptr, const Size size, vk::IndexType indexType) noexcept : indices(ptr), size(size), indexType(indexType) {}

			void* indices = nullptr;
			Size size = 0;
			vk::IndexType indexType = vk::IndexType::eUint32;
		};

	public:
		struct Parameter final :private ParameterBase {
			explicit Parameter(std::vector<uint32_t>* v) : ParameterBase(v->data(), static_cast<Size>(sizeof(v->at(0))* v->size()), vk::IndexType::eUint32), bufferSize(sizeof(v->at(0))* v->size()) {}
			explicit Parameter(std::vector<uint16_t>* v) : ParameterBase(v->data(), static_cast<Size>(sizeof(v->at(0))* v->size()), vk::IndexType::eUint16), bufferSize(sizeof(v->at(0))* v->size()) {}

		public:
			Size bufferSize = 0;
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;

		private:
			friend IndexBuffer;

		};

	public:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		/*
		If an exception is thrown,
		it is possible that the create function will leave the value in this->info.
		However, the create function is safe because it is only called by the constructor.
		*/
		explicit IndexBuffer(ManagerPassKey, const Data::IndexBufferData& data, const Parameter& parameter);

		~IndexBuffer() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(IndexBuffer);

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		void create(const Data::IndexBufferData& data, const Parameter& parameter);

	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::IndexBufferInfo& get() const noexcept;

	private:
		Data::IndexBufferInfo info{};

	};

}
