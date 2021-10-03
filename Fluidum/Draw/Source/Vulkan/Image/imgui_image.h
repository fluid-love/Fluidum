#pragma once

#include "../../Common/include.h"
#include "../Base/base.h"

//前方宣言
namespace FDR::Internal {
	struct ImGuiImageGetter;
}

namespace FDR::Internal {

	//destructorで自動的にImageを解放する
	class ImGuiImageData final {
	public:
		explicit ImGuiImageData(ImTextureID image, std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>>&& keys);

		//例外が出るとlogに追加してabort
		~ImGuiImageData() noexcept;

	public:
		ImTextureID image;
		std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>> keys;
	};

	class ImGuiImage final {
	public:
		explicit ImGuiImage(ImTextureID image, std::pair<FVK::TextureKey<std::string>, FVK::ImGuiImageKey<std::string>>&& keys);
		~ImGuiImage() = default;
		FluidumUtils_Class_Default_CopyMove(ImGuiImage)

	private:
		std::shared_ptr<ImGuiImageData> data;
	private:
		friend struct ImGuiImageGetter;
	};

	ImGuiImage createImGuiImage(const char* filePath);

	struct ImGuiImageGetter final {
		_NODISCARD ImTextureID get(const ImGuiImage& image) const noexcept;
	};
}

namespace FDR {
	using Internal::ImGuiImage;
}

//ImGui::Image,ImGui::ImageButtonのオーバーロードを提供する
namespace ImGui {
	void Image(const FDR::ImGuiImage& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));

	bool ImageButton(const FDR::ImGuiImage& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
}