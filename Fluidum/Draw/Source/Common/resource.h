#pragma once

#include "include.h"

#ifdef FluidumDraw_Vulkan

namespace FDR::Internal {

#ifdef Fluidum_Path
	constexpr inline const char* VertShaderFilePath = Fluidum_Path "Resources/Shaders/vert.spv";
	constexpr inline const char* FragShaderFilePath = Fluidum_Path "Resources/Shaders/frag.spv";
#else
	constexpr inline const char* VertShaderFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 5, FU::File::PathArg("/Resources/Shaders/vert.spv")>();
	constexpr inline const char* FragShaderFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 5, FU::File::PathArg("/Resources/Shaders/frag.spv")>();
#endif	


#ifdef Fluidum_Path
	constexpr inline const char* ImGuiDefaultFontFilePath = Fluidum_Path "Resources/Fonts/clamp/clamp-1m-w3-bold.ttf";
	constexpr inline const char* ImGuiDefaultIconFontFilePath = Fluidum_Path "Resources/Fonts/material-design-icons/MaterialIcons-Regular.ttf";
	constexpr inline const char* ImGuiDefaultIconFont2FilePath = Fluidum_Path "/Resources/Fonts/FontAwesome/fa-solid-900.ttf";
#else
	constexpr inline const char* ImGuiDefaultFontFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 5, FU::File::PathArg("/Resources/Fonts/clamp/clamp-1m-w1-bold.ttf")>();
	constexpr inline const char* ImGuiDefaultIconFontFilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 5, FU::File::PathArg("/Resources/Fonts/material-design-icons/MaterialIcons-Regular.ttf")>();
	constexpr inline const char* ImGuiDefaultIconFont2FilePath = FU::File::MakePath<FU::File::PathArg(__FILE__), 5, FU::File::PathArg("/Resources/Fonts/FontAwesome/fa-solid-900.ttf")>();
#endif	

}

#endif