#pragma once

#include "../Common/include.h"
#include "backend.h"

namespace FVK::Internal {

	class FvkImGui final {
	public:
		struct CreateInfo {
			std::optional<Key::WindowVariantKey> windowKey = std::nullopt;
			std::optional<Key::InstanceVariantKey> instanceKey = std::nullopt;
			std::optional<Key::PhysicalDeviceVariantKey> physicalDeviceKey = std::nullopt;
			std::optional<Key::LogicalDeviceVariantKey> logicalDeviceKey = std::nullopt;
			std::optional<Key::QueueVariantKey> presentQueueKey = std::nullopt;
			std::optional<Key::DescriptorPoolVariantKey> descriptorPoolKey = std::nullopt;
			std::optional<VkSampleCountFlagBits> msaaSamples = std::nullopt;
			UI32 minImageCount = 0;
			UI32 imageCount = 0;
			std::optional<Key::RenderPassVariantKey> renderPassKey = std::nullopt;
		};

		enum class GlyphRangesType : UT {
			Default,
			Korean,
			Japanese,
			ChineseFull,
			ChineseSimplifiedCommon,
			Cyrillic,
			Thai,
			Vietnamese,
		};

		struct FontInfo {
			const char* filePath = "";
			float sizePixels = 20.0f;
			std::optional<ImFontConfig*> config = std::nullopt;
			std::optional<std::variant<const ImWchar*, GlyphRangesType>> glyphRanges = std::nullopt;
		};

		struct Parameter {
			CreateInfo* pInfo = nullptr;
			std::optional<Key::CommandPoolVariantKey> commandPoolKey = std::nullopt;
			std::optional<Key::QueueVariantKey> graphicsQueueKey = std::nullopt;
			FontInfo* pFontInfos = nullptr;
			UI32 fontInfoCount = 0;
		};

	public:
		/*
		Exception:
			std::exception
		*/
		//basic
		explicit FvkImGui(ManagerPassKey, const Data::ImGuiData& data, const Parameter& parameter);

		~FvkImGui() noexcept = default;
		FluidumUtils_Class_Default_CopyMove(FvkImGui)

	private:
		/*
		Exception:
			std::exception
		*/
		//basic
		void create(const Data::ImGuiData& data, const Parameter& parameter);

	public:
		/*
		Exception:
			FailedToDestroy
		*/
		//no-exception-safety
		//It depends on the implementation of imgui.
		//Probably no-throw.
		void destroy();

	public:
		//no-throw
		[[nodiscard]] const Data::ImGuiInfo& get() const noexcept;

	private:
		Data::ImGuiInfo info{};

	};

}
