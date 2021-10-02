#include "guitext.h"

Fluidum::Utils::GuiText::Text::Text(const std::string text)
	: text(text)
{}

Fluidum::Utils::GuiText::Text::Text(std::string&& text) noexcept
	: text(text)
{}

Fluidum::Utils::GuiText::Text::Text(const char* text)
	: text(text)
{}

void Fluidum::Utils::GuiText::Text::operator=(const std::string& text) {
	this->text = text;
}

void Fluidum::Utils::GuiText::Text::operator=(std::string&& text) noexcept {
	this->text = std::move(text);
}

Fluidum::Utils::GuiText::Text::operator const char* () const noexcept {
	return this->text.c_str();
}

