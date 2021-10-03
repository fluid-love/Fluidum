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
			uint32_t minImageCount = 0;
			uint32_t imageCount = 0;
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
			uint32_t fontInfoCount = 0;
		};

	public:
		explicit FvkImGui(ManagerPassKey, const Data::ImGuiData& data, const Parameter& parameter);
		~FvkImGui() = default;
		FluidumUtils_Class_Default_CopyMove(FvkImGui)

	private:
		void create(const Data::ImGuiData& data, const Parameter& parameter);
	public:
		_NODISCARD const Data::ImGuiInfo& get() const noexcept;
		void destroy();

	private://data
		Data::ImGuiInfo info = {};
	};



}
