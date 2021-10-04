#pragma once

#include "../../Common/common.h"

namespace FS {

	namespace Utils {
		enum class PopupSelectIconType {
			None,
			Info,
			Warning,
		};
	}

	class PopupSelect final : public Scene {
	public:
		template<typename... String>
		explicit PopupSelect(
			const FD::GuiRead* const guiRead,
			const Utils::PopupSelectIconType type,
			const FU::GuiText::Text& message,
			const String&... buttonLabels
		) :
			guiRead(guiRead),
			message(message),
			buttonLabels({ buttonLabels... }),
			image(createImage(type))
		{
			GLog.add<FD::Log::Type::None>("Construct PopupSelectScene.");

			this->init();

			style.buttonSize = this->calcButtonSize(sizeof...(String));
		}
		void Constructor(FD::GuiRead);

		~PopupSelect() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::GuiRead* const guiRead;

		const std::string message;
		const std::vector<FU::GuiText::Text> buttonLabels;

		const std::optional<FDR::ImGuiImage> image;

		FD::Text::PopupSelect text{};

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};
			ImVec2 buttonSize{};
		}style;

	private:
		void messageGui();
		void button();

	private:
		ImVec2 calcButtonSize(const uint16_t size);
		std::optional<FDR::ImGuiImage> createImage(const Utils::PopupSelectIconType type);
		void init();
	};
}