#pragma once

#include "../Common/include.h"

namespace FVK::Internal {

	class Surface final {
	public:
		struct Parameter {
			std::optional<Key::WindowVariantKey> windowKey = std::nullopt;
			std::optional<Key::InstanceVariantKey> instanceKey = std::nullopt;
		};

	public:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		explicit Surface(ManagerPassKey, const Data::SurfaceData& data, const Parameter& parameter);
		
		~Surface() = default;
		FluidumUtils_Class_Default_CopyMove(Surface);

	private:
		/*
		Exception:
			FailedToCreate
		*/
		//strong
		void create(const Data::SurfaceData& data, const Parameter& parameter);

	public:
		//no-throw
		const Data::SurfaceInfo& get() const noexcept;

	public:
		//no-throw
		void destroy() noexcept;

	private:
		//no-throw
		void fillInfo(const Data::SurfaceData& data) noexcept;

	private:
		Data::SurfaceInfo info{};

	};

}