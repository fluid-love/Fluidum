#include "fluidum_imgui.h"

#include "../Buffer/buffer.h"

FVK::Internal::FvkImGui::FvkImGui(ManagerPassKey, const Data::ImGuiData& data, const Parameter& parameter) {
	this->create(data, parameter);
}

const ImWchar* getGlyphRanges(const ImGuiIO& io, const FVK::Internal::FvkImGui::GlyphRangesType type) {
	using enum FVK::Internal::FvkImGui::GlyphRangesType;
	if (type == Japanese)
		return io.Fonts->GetGlyphRangesJapanese();

	return io.Fonts->GetGlyphRangesDefault();
}

void FVK::Internal::FvkImGui::create(const Data::ImGuiData& data, const Parameter& parameter) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	try {
		this->info.fonts.resize(parameter.fontInfoCount);

		for (UI32 i = 0; i < parameter.fontInfoCount; i++) {
			assert(parameter.pFontInfos);
			auto* fontInfo = parameter.pFontInfos;

			const ImWchar* ranges{};
			if (fontInfo[i].glyphRanges.has_value()) {
				if (fontInfo[i].glyphRanges.value().index() == 0)
					ranges = std::get<0>(fontInfo[i].glyphRanges.value());
				else
					ranges = getGlyphRanges(io, std::get<1>(fontInfo[i].glyphRanges.value()));
			}
			else {
				ranges = nullptr;
			}

			this->info.fonts.at(i) = io.Fonts->AddFontFromFileTTF(
				fontInfo[i].filePath,
				fontInfo[i].sizePixels,
				fontInfo[i].config.has_value() ? fontInfo[i].config.value() : nullptr,
				ranges
			);
		}

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForVulkan(data.get<FvkType::Window>().window, true);
	}
	catch (...) {
		ImGui::DestroyContext();

		std::rethrow_exception(std::current_exception());
	}

	try {
		ImGui_ImplVulkan_InitInfo init_info{};
		init_info.Instance = data.get<FvkType::Instance>().instance;
		init_info.PhysicalDevice = data.get<FvkType::PhysicalDevice>().physicalDevice;
		init_info.Device = data.get<FvkType::LogicalDevice>().device;
		init_info.QueueFamily = data.get<FvkType::PhysicalDevice>().graphicsFamily;
		init_info.Queue = data.get<FvkType::Queue_Vector>().at(1).get().queue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;
		init_info.Allocator = nullptr;
		if (parameter.pInfo->msaaSamples.has_value())
			init_info.MSAASamples = parameter.pInfo->msaaSamples.value();

		init_info.MinImageCount = parameter.pInfo->minImageCount;
		init_info.ImageCount = parameter.pInfo->imageCount;
		init_info.CheckVkResultFn = nullptr;
		ImGui_ImplVulkan_Init(&init_info, data.get<FvkType::RenderPass>().renderPass);
	}
	catch (...) {
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		std::rethrow_exception(std::current_exception());
	}

	try {
		ImGui::StyleColorsDark();
		ImGuiStyle* style = &ImGui::GetStyle();
		style->AntiAliasedLines = true;
		style->AntiAliasedFill = true;
		style->AntiAliasedLinesUseTex = true;

		vk::CommandBuffer commandBuffer = Buffer::beginSingleTimeCommands(data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool);
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
		Buffer::endSingleTimeCommands(commandBuffer, data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool, data.get<FvkType::Queue_Vector>().at(0).get().queue);
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}
	catch (...) {
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		std::rethrow_exception(std::current_exception());
	}

	//no-throw
	this->info.device = data.get<FvkType::LogicalDevice>().device;
	this->info.descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;
}

const FVK::Internal::Data::ImGuiInfo& FVK::Internal::FvkImGui::get() const noexcept {
	assert(info.descriptorPool && info.device);
	return this->info;
}

void FVK::Internal::FvkImGui::destroy() {
	try {
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	catch (...) {
		Exception::throwFailedToDestroy();
	}
}

