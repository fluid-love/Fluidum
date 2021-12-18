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

	this->info.fonts.resize(parameter.fontInfoCount);

	///*iconÇfontÇ…ëgÇ›çûÇﬁ*/
	for (uint32_t i = 0; i < parameter.fontInfoCount; i++) {
		assert(parameter.pFontInfos);
		auto* fontInfo = parameter.pFontInfos;

		const ImWchar* ranges;
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

	ImGui_ImplVulkan_InitInfo init_info = {};
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


	ImGui::StyleColorsDark();
	ImGuiStyle* style = &ImGui::GetStyle();
	style->AntiAliasedLines = true;
	style->AntiAliasedFill = true;
	style->AntiAliasedLinesUseTex = true;

	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.200f, 0.200f, 0.200f, 0.800f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.016f, 0.016f, 0.016f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.180f, 0.180f, 0.180f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.26f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.060f, 0.060f, 0.060f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.100f, 0.100f, 0.100f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.113f, 0.113f, 0.113f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.165f, 0.165f, 0.165f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 0.0f;

	vk::CommandBuffer commandBuffer = Buffer::beginSingleTimeCommands(data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool);
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	Buffer::endSingleTimeCommands(commandBuffer, data.get<FvkType::LogicalDevice>().device, data.get<FvkType::CommandPool>().commandPool, data.get<FvkType::Queue_Vector>().at(0).get().queue);
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	this->info.device = data.get<FvkType::LogicalDevice>().device;
	this->info.descriptorPool = data.get<FvkType::DescriptorPool>().descriptorPool;

}

const FVK::Internal::Data::ImGuiInfo& FVK::Internal::FvkImGui::get() const noexcept {
	assert(info.descriptorPool && info.device);
	return this->info;
}

void FVK::Internal::FvkImGui::destroy() {
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

