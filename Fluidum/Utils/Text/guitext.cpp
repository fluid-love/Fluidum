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

#include <windows.h>
std::wstring FU::Text::utf8ToUtf16(const std::string& str) {
	if (str.empty())
		return std::wstring{};
	int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0);
	std::wstring result(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), size);
	return result;
}