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
		explicit Surface(ManagerPassKey, const Data::SurfaceData& data, const Parameter& parameter);
		~Surface() = default;
		FluidumUtils_Class_Default_CopyMove(Surface)

	private:
		void create(const Data::SurfaceData& data, const Parameter& parameter);
	public:
		const Data::SurfaceInfo& get() const noexcept;
		void destroy();

	private:
		void fillInfo(const Data::SurfaceData& data) noexcept;

	private:
		Data::SurfaceInfo info = {};
	};
}