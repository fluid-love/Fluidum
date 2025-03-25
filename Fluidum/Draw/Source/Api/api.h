#pragma once

#ifdef FluidumDraw_Vulkan
#include "../Vulkan/include.h"
#endif

#include "../Api/using.h"

#define FluidumDrawAPI

//Main
namespace FDR {

	//initialize
	/*
	Create the main window and set up ImGui.
	Then call the function in the argument at the appropriate place every frame.

	Three fonts are loaded using freetype.

	ImGui can be used, so they can be written from ImGui::Begin().
	ImGui::Render and ImGui::NewFrame do not need to be called; they are handled by FluidumDraw.
	*/

	//FullscreenWindow
	/*
	arg:
		title           : The string displayed in the window's titlebar
		windowCloseFlag : When this variable is true, exit the loop.
	*/
	FluidumDrawAPI void mainLoop(const char* title, void(*function)(), bool* const windowCloseFlag);

}

//Function
namespace FDR {

	//set callback
	/*
	arg:
		callback : callback
	*/
	FluidumDrawAPI void setMessengerCallbackFunction(const MessengerCallbackType callback);

}

//Main Window
namespace FDR {

	//get main window size
	/*
	return:
		{width, height}
	*/
	FluidumDrawAPI [[nodiscard]] std::pair<IF32, IF32> getWindowSize();

	FluidumDrawAPI [[nodiscard]] std::pair<IF32, IF32> getFullscreenWindowSize();

	//get main window pos
	/*
	return:
		{x, y}
	*/
	FluidumDrawAPI [[nodiscard]] std::pair<IF32, IF32> getWindowPos();

	//set window size
	/*
	arg:
		width  : width
		height : height
	*/
	FluidumDrawAPI void setWindowSize(const IF32 width, const IF32 height);
	/*
	arg:
		size {width, height}
	*/
	FluidumDrawAPI void setWindowSize(const ImVec2& size);

	//set window pos
	/*
	arg:
		x : x
		y : y
	*/
	FluidumDrawAPI void setWindowPos(const IF32 x, const IF32 y);
	/*
	arg:
		pos : pos
	*/
	FluidumDrawAPI void setWindowPos(const ImVec2& pos);


	/*
	Set window pos and size.

	Args:
		width  : width(size)
		height : height(size)
		x	   : x(pos)
		y	   : y(pos)
	*/
	FluidumDrawAPI void setWindowPosSize(const IF32 x, const IF32 y, const IF32 width, const IF32 height);

	/*
	Set window pos and size.

	Args:
		width  : width(size)
		height : height(size)
		x	   : x(pos)
		y	   : y(pos)

	Note:
		It will be changed at the appropriate time.
	*/
	FluidumDrawAPI void setWindowPosSize_timing(const IF32 x, const IF32 y, const IF32 width, const IF32 height);

	/*
	Set window size.

	Args:
		width  : width of window.
		height : height of window.

	Note:
		It will be changed at the appropriate time.
	*/
	FluidumDrawAPI void setWindowSize_timing(const IF32 width, const IF32 height);

	/*
	Set window size.

	Args:
		size : { width, height }

	Note:
		It will be changed at the appropriate time.
	*/
	FluidumDrawAPI void setWindowSize_timing(const ImVec2& size);

	/*
	Set window pos.

	Args:
		width  : posX
		height : posY

	Note:
		It will be changed at the appropriate time.
	*/
	FluidumDrawAPI void setWindowPos_timing(const IF32 x, const IF32 y);

	/*
	Set window pos.

	Args:
		pos  : { posX, posY }

	Note:
		It will be changed at the appropriate time.
	*/
	FluidumDrawAPI void setWindowPos_timing(const ImVec2& pos);

	//minimize window
	FluidumDrawAPI void minimizeWindow();

	//fullcreen window
	FluidumDrawAPI void fullscreenWindow();

	//restore window
	FluidumDrawAPI void restoreWindow();

	FluidumDrawAPI [[nodiscard]] bool isWindowFocused();

	FluidumDrawAPI [[nodiscard]] bool isWindowFullscreen();

	FluidumDrawAPI void setWindowWidthMaxLimit(const IF32 width);

	FluidumDrawAPI void setWindowHeightMaxLimit(const IF32 height);

	/*
	Args:
		r	  : red   0-255
		g	  : green 0-255
		b	  : blue  0-255
		alpha : alpha 0-255
	*/
	FluidumDrawAPI void setTransparentWindow(const I32 r, const I32 g, const I32 b, const I32 alpha);
	FluidumDrawAPI void unsetTransparentWindow();

}

//viewport scissor
namespace FDR {

	FluidumDrawAPI void setViewportWidthHeight(const float width, const float height);

	FluidumDrawAPI void setScissorExtent(const UI32 width, const UI32 height);

	FluidumDrawAPI void setScissorOffset(const I32 x, const I32 y);

}

//RenderPass
namespace FDR {

	FluidumDrawAPI void setClearColor(const float r, const float g, const float b, const float a);

}

//Image
namespace FDR {

	//Create and retrieve an Image that can be used with ImGui
	/*
	The return value is ImGuiImage, which has ImTextureID inside.
	ImGui::Image(ImTextureID,...) ImGui::Image(FDR::ImGuiImage,...) is provided as a function overload. as an overload of the ImGui::Image function.
	This is because getting the ImTextureID(void*) inside ImGuiImage may point to an invalid address, since it releases resources as follows.
	I use shared_ptr internally. The resource is released when the number of shares reaches zero.

	Translated with www.DeepL.com/Translator (free version)

	arg:
		filePath : Path of the image to load(.png .jpeg .jpg .ppm ...).

	return:
		image
	*/
	FluidumDrawAPI ImGuiImage createImGuiImage(const char* filePath);

}

//Font
namespace FDR {

	//Get the small size of the font provided by default for ImGui::PushFont;
	FluidumDrawAPI [[nodiscard]] ImFont* getDefaultFontMiniSize();

}