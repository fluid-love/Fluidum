#pragma once

#include "../Common/include.h"
#include "buffer.h"

namespace FVK::Internal {

	class VertexBuffer final {
	private:
		struct ParameterBase {
			ParameterBase() = delete;
			explicit ParameterBase(void* ptr, const Size size) noexcept : vertices(ptr), size(size) {}

			Size size;
			void* vertices = nullptr;
		};

	public:
		struct Parameter final :private ParameterBase {
			template<typename T>
			Parameter(std::vector<T>* v) : ParameterBase(v->data(), sizeof(v->at(0)) * v->size()), bufferSize(sizeof(v->at(0))* v->size()) {}

		public:
			Size bufferSize = 0;
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;

		private:
			friend VertexBuffer;
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
		explicit VertexBuffer(ManagerPassKey, const Data::VertexBufferData& data,const Parameter& parameter);
	
		~VertexBuffer() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(VertexBuffer)

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//basic
		void create(const Data::VertexBufferData& data,const Parameter& parameter);
	
	public:
		//no-throw
		void destroy() noexcept;

	public:
		//no-throw
		[[nodiscard]] const Data::VertexBufferInfo& get() const noexcept;

	private:
		Data::VertexBufferInfo info{};

	};

}
