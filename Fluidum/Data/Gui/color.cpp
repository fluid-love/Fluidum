#include "color.h"

namespace FD {
	Style::StyleType getType() {
		std::ifstream ifs(Internal::Resource::GuiStyleFilePath, std::ios::in);

		std::string data{};
		std::getline(ifs, data);

		if (data == "Default")
			return Style::StyleType::Default;
		else if (data == "Dark")
			return Style::StyleType::Dark;
		else if (data == "Light")
			return Style::StyleType::Light;
		else
			throw std::runtime_error("Failed to read GuiStyleType.");

		return{};
	}
}

FD::StyleColorRead::StyleColorRead(Internal::PassKey)
	: type(getType())
{


}

FD::Style::StyleType FD::StyleColorRead::getType() const noexcept {
	return this->type;
}
