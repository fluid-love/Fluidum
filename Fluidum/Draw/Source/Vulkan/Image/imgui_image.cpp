#include "imgui_image.h"

namespace FDR::Internal {

	//Generates a string with no duplicate keys.
	class ImGuiImageKeyMaker final {
	public:
		static std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>> make() noexcept {
			if (number == std::numeric_limits<FDR::Size>::max()) {
				FDR::Internal::GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, "Failed to make TextureKey and ImGuiImageKey. ImGuiImageKeyMaker::number exceeds max limit.");
				std::terminate();
			}
			number++;
			return std::make_pair(FVK::TextureKey<std::string>{std::string("_Tx_" + std::to_string(number))}, FVK::ImGuiImageKey<std::string>{std::string("_II_" + std::to_string(number))});
		}

	private:
		static inline Size number = 0;

	};
}


FDR::Internal::ImGuiImageData::ImGuiImageData(ImTextureID image, std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>>&& keys)
	: image(image), keys(std::move(keys))
{}

FDR::Internal::ImGuiImageData::~ImGuiImageData() noexcept {
	//noexcept
	//����Ă��Ȃ�->���ɍ폜���ꂽ
	//�폜����Ă���������Ŏ����I�Ƀ��\�[�X�͉������Ă��邽�߉������Ȃ��Ă悢
	if (!FVK::isFluidumVKInitialized()) {
		return;
	}

	try {
		FVK::destroyTexture(keys.first);
		FVK::destroyImGuiImage(keys.second);
	}
	//������->ImGuiImage���쐬->ImGuiImage��copy->�I��(���\�[�X����������)->������񏉊���->�������Ă��邪��̏������Ɉ��������炸abort�ɂȂ��Ă��܂��D
	//catch (const FVK::Exception::NotFound&) {
	//	return;
	//}
	catch (const std::exception& e) {
		GMessenger.add<FU::Log::Type::Error>(__FILE__, __LINE__, e.what());
		std::terminate();
	}
	catch (...) {
		std::cerr << "Error: " << __FILE__ << std::endl;
		std::terminate();
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

