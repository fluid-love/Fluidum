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
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		explicit ImGuiImage(ManagerPassKey, const Data::ImGuiImageData& data, const Parameter& parameter);

		~ImGuiImage() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(ImGuiImage)

	private:
		/*
		Exception:
			std::exception
			FailedToCreate
		*/
		//strong
		void create(const Data::ImGuiImageData& data, const Parameter& parameter);

	public:
		/*
		Exception:
			FailedToDestroy
		*/
		//strong
		void destroy();

	public:
		[[nodiscard]] const Data::ImGuiImageInfo& get() const noexcept;

	private:
		Data::ImGuiImageInfo info{};

	};

}
