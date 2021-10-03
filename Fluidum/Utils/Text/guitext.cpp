#include "guitext.h"

FU::GuiText::Text::Text(const std::string text)
	: text(text)
{}

FU::GuiText::Text::Text(std::string&& text) noexcept
	: text(text)
{}

FU::GuiText::Text::Text(const char* text)
	: text(text)
{}

void FU::GuiText::Text::operator=(const std::string& text) {
	this->text = text;
}

void FU::GuiText::Text::operator=(std::string&& text) noexcept {
	this->text = std::move(text);
}

FU::GuiText::Text::operator const char* () const noexcept {
	return this->text.c_str();
}

