#pragma once

#include <string>
#include "../Class/class.h"

namespace FU::GuiText{

	class Text final {
	public:
		explicit Text(const std::string text);
		explicit Text(std::string&& text) noexcept;
		explicit Text(const char* text);

		FluidumUtils_Class_Default_ConDestructor(Text)
			FluidumUtils_Class_Default_CopyMove(Text)

	private:
		std::string text{};
	public:
		void operator=(const std::string& text);
		void operator=(std::string&& text) noexcept;

		operator const char* () const noexcept;
	};


}