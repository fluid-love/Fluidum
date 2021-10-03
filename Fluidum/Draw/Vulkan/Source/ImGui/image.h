#pragma once

#include "../Common/include.h"
#include "backend.h"

namespace FVK::Internal {

	class ImGuiImage final {
	public:

		struct Parameter {
			std::optional<Key::SamplerVariantKey> samplerKey = std::nullopt;
			std::optional<Key::TextureVariantKey> textureKey = std::nullopt;
			std::optional<Key::ImGuiVariantKey> imguiKey = std::nullopt;
		};

	public:
		explicit ImGuiImage(ManagerPassKey, const Data::ImGuiImageData& data, const Parameter& parameter);
		~ImGuiImage() = default;
		FluidumUtils_Class_Default_CopyMove(ImGuiImage)

	private:
		void create(const Data::ImGuiImageData& data, const Parameter& parameter);
	public:
		_NODISCARD const Data::ImGuiImageInfo& get() const noexcept;
		void destroy();

	private://data
		Data::ImGuiImageInfo info = {};
	};



}
