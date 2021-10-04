#pragma once

#include "../../Common/common.h"

namespace FS::Utils {

	class Message final : public Scene {
	public:
		explicit Message(
			const FU::GuiText::Text& message,
			const ImVec2& pos,
			const ImVec4& bgColor = { 0.11f,0.01f,0.01f,1.0f }
		);
		void Constructor();

		~Message() noexcept;

	public:
		virtual void call() override;

	private://data
		const FU::GuiText::Text message;
		const ImVec2 pos;
		const ImVec4 bgColor;
	};
}