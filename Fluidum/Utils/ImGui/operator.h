#pragma once

#include <imgui.h>

//usage
/*
never use "using namespace" in header file. 
e.g. 
//header.h
using namespace ::Fluidum::Utils::ImGui::Operators;

void test(){
	ImVec2 a,b;
	a + b;
}

use in .cpp / inside a function
void test(){
	using namespace ::Fluidum::Utils::ImGui::Operators;
	//foo
*/

namespace Fluidum::Utils::ImGui::Operators {

	/*
	l.x + r.x
	l.y + r.y
	*/
	ImVec2 operator+(const ImVec2& l, const ImVec2& r) noexcept;


	/*
	l.x - r.x
	l.y - r.y
	*/
	ImVec2 operator-(const ImVec2& l, const ImVec2& r) noexcept;


	/*
	l.x * r.x
	l.y * r.y
	*/
	ImVec2 operator*(const ImVec2& l, const ImVec2& r) noexcept;


	/*
	l.x / r.x
	l.y / r.y
	*/
	ImVec2 operator/(const ImVec2& l, const ImVec2& r) noexcept;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/*
	l.x + r
	l.y + r
	*/
	ImVec2 operator+(const ImVec2& l, const float r) noexcept;


	/*
	l.x - r
	l.y - r
	*/
	ImVec2 operator-(const ImVec2& l, const float r) noexcept;


	/*
	l.x * r
	l.y * r
	*/
	ImVec2 operator*(const ImVec2& l, const float r) noexcept;


	/*
	l.x / r
	l.y / r
	*/
	ImVec2 operator/(const ImVec2& l, const float r) noexcept;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/*
	l.x + r.x
	l.y + r.y
	l.z + r.z
	l.w + r.w
	*/
	ImVec4 operator+(const ImVec4& l, const ImVec4& r) noexcept;


	/*
	l.x - r.x
	l.y - r.y
	l.z - r.z
	l.w - r.w
	*/
	ImVec4 operator-(const ImVec4& l, const ImVec4& r) noexcept;


	/*
	l.x * r.x
	l.y * r.y
	l.z * r.z
	l.w * r.w
	*/
	ImVec4 operator*(const ImVec4& l, const ImVec4& r) noexcept;


	/*
	l.x / r.x
	l.y / r.y
	l.z / r.z
	l.w / r.w
	*/
	ImVec4 operator/(const ImVec4& l, const ImVec4& r) noexcept;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/*
	l.x + r
	l.y + r
	l.z + r
	l.w + r
	*/
	ImVec4 operator+(const ImVec4& l, const float r) noexcept;


	/*
	l.x - r
	l.y - r
	l.z - r
	l.w - r
	*/
	ImVec4 operator-(const ImVec4& l, const float r) noexcept;


	/*
	l.x * r
	l.y * r
	l.z * r
	l.w * r
	*/
	ImVec4 operator*(const ImVec4& l, const float r) noexcept;


	/*
	l.x / r
	l.y / r
	l.z / r
	l.w / r
	*/
	ImVec4 operator/(const ImVec4& l, const float r) noexcept;


}

