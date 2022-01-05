#include "imgui_image.h"

//被らないTextureKeyとImGuiImageKeyを作る
class ImGuiImageKeyMaker final {
public:
	static std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>> make() {
		if (number == std::numeric_limits<std::size_t>::max())
			throw std::runtime_error("Failed to make TextureKey and ImGuiImageKey. ImGuiImageKeyMaker::number exceeds max limit.");
		number++;
		return std::make_pair(FVK::TextureKey<std::string>{std::string("_Tx_" + std::to_string(number))}, FVK::ImGuiImageKey<std::string>{std::string("_II_" + std::to_string(number))});
	}
private:
	static inline std::size_t number = 0;
};

FDR::Internal::ImGuiImageData::ImGuiImageData(ImTextureID image, std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>>&& keys)
	: image(image), keys(std::move(keys))
{}

FDR::Internal::ImGuiImageData::~ImGuiImageData() noexcept {
	//noexcept
	//作られていない->既に削除された
	//削除されていたら内部で自動的にリソースは解放されているため何もしなくてよい
	if (!FVK::isFluidumVKInitialized()) {
		return;
	}

	try {
		FVK::destroyTexture(keys.first);
		FVK::destroyImGuiImage(keys.second);
	}
	//初期化->ImGuiImageを作成->ImGuiImageをcopy->終了(リソースが解放される)->もう一回初期化->解放されているが上の条件式に引っかからずabortになってしまう．
	//catch (const FVK::Exception::NotFound&) {
	//	return;
	//}
	catch (const std::exception& e) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, e.what());
		abort();
	}
	catch (...) {
		std::cerr << "Error: " << __FILE__ << std::endl;
		abort();
	}
}

FDR::Internal::ImGuiImage::ImGuiImage(ImTextureID image, std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>>&& keys)
	: data(std::make_shared<ImGuiImageData>(image, std::move(keys)))
{}

FDR::Internal::ImGuiImage FDR::Internal::createImGuiImage(const char* filePath) {
	auto keyPair = ImGuiImageKeyMaker::make();
	{
		FVK::TextureParameter param;
		param.filePath = filePath;
		param.physicalDeviceKey = BasePhysicalDeviceKey;
		param.logicalDeviceKey = BaseDeviceKey;
		param.commandPoolKey = BaseCommandPoolKey;
		param.graphicsQueueKey = BaseGraphicsQueueKey;

		FVK::createTexture(keyPair.first, param);
	}
	ImTextureID image;
	{
		FVK::ImGuiImageParameter param;
		param.imguiKey = BaseImGuiKey;
		param.samplerKey = BaseSamplerKey;
		param.textureKey = keyPair.first;
		image = FVK::createImGuiImage(keyPair.second, param);
	}

	return ImGuiImage(image, std::move(keyPair));
}

ImTextureID FDR::Internal::ImGuiImageGetter::get(const ImGuiImage& image) const noexcept {
	return image.data.get()->image;
}

void ImGui::Image(const FDR::ImGuiImage& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) {
	FDR::Internal::ImGuiImageGetter getter{};
	const ImTextureID id = getter.get(image);
	ImGui::Image(id, size, uv0, uv1, tint_col, border_col);
}

bool ImGui::ImageButton(const FDR::ImGuiImage& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) {
	FDR::Internal::ImGuiImageGetter getter{};
	const ImTextureID id = getter.get(image);
	return ImGui::ImageButton(id, size, uv0, uv1, frame_padding, bg_col, tint_col);
}

