#include "operator.h"

ImVec2 Fluidum::Utils::ImGui::Operators::operator+(const ImVec2& l, const ImVec2& r) noexcept {
	return ImVec2(l.x + r.x, l.y + r.y);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator-(const ImVec2& l, const ImVec2& r) noexcept {
	return ImVec2(l.x - r.x, l.y - r.y);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator*(const ImVec2& l, const ImVec2& r) noexcept {
	return ImVec2(l.x * r.x, l.y * r.y);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator/(const ImVec2& l, const ImVec2& r) noexcept {
	return ImVec2(l.x / r.x, l.y / r.y);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator+(const ImVec2& l, const float r) noexcept {
	return ImVec2(l.x + r, l.y + r);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator-(const ImVec2& l, const float r) noexcept {
	return ImVec2(l.x - r, l.y - r);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator*(const ImVec2& l, const float r) noexcept {
	return ImVec2(l.x * r, l.y * r);
}

ImVec2 Fluidum::Utils::ImGui::Operators::operator/(const ImVec2& l, const float r) noexcept {
	return ImVec2(l.x / r, l.y / r);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator+(const ImVec4& l, const ImVec4& r) noexcept {
	return ImVec4(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator-(const ImVec4& l, const ImVec4& r) noexcept {
	return ImVec4(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator*(const ImVec4& l, const ImVec4& r) noexcept {
	return ImVec4(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator/(const ImVec4& l, const ImVec4& r) noexcept {
	return ImVec4(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator+(const ImVec4& l, const float r) noexcept {
	return ImVec4(l.x + r, l.y + r, l.z + r, l.w + r);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator-(const ImVec4& l, const float r) noexcept {
	return ImVec4(l.x - r, l.y - r, l.z - r, l.w - r);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator*(const ImVec4& l, const float r) noexcept {
	return ImVec4(l.x * r, l.y * r, l.z * r, l.w * r);
}

ImVec4 Fluidum::Utils::ImGui::Operators::operator/(const ImVec4& l, const float r) noexcept {
	return ImVec4(l.x / r, l.y / r, l.z / r, l.w / r);
}










