/*
対処できる例外はない
例外はログをMessengerに渡したあと，一律に"FDR::Exception::Error"を投げる．
FDR::Exception::Errorはstd::nasted_exceptionから派生している．
*/

#pragma once

#ifdef FluidumDraw_Vulkan
#include "../Vulkan/include.h"
#endif

#include "../Api/using.h"

#define FluidumDrawAPI

//Main
namespace FDR {

	//初期化する
	/*
	メインウィンドウを作成し，ImGuiをセットアップする.
	そして，引数のfunctionを毎フレーム適切な場所で呼び出す

	フォントはfreetypeを利用して3つロードされる.
		fa_

	注意：setMessengerCallbackFunctionを除き，全ての関数はmainLoopないで呼ばなければならない

	引数:
		function 呼び出される関数
				 ImGuiの機能は利用可能なので，ImGui::Begin()から記述してよい．
				 ImGui::RenderやImGui::NewFrameは呼び出さなくてよい．FluidumDraw側で処理される．

	*/

	//フルスクリーンウィンドウ
	/*
	引数:
		title           windowのtitlebarに表示される文字列
		windowCloseFlag この変数がtrueになるとloopを抜ける
	*/
	FluidumDrawAPI void mainLoop(const char* title, void(*function)(), bool* const windowCloseFlag);

}

//Function
namespace FDR {

	//Messageが発生したときに一回呼び出される
	/*
	引数はconst std::string&であり，TimeとLogType付のメッセージが入ってくる．

	引数:
		callback 呼び出される関数

	*/
	FluidumDrawAPI void setMessengerCallbackFunction(const MessengerCallbackType callback);


}

//Window
namespace FDR {

	//Windowのサイズを取得
	/*
	戻り値: {width, height}
	*/
	FluidumDrawAPI _NODISCARD std::pair<int32_t, int32_t> getWindowSize();


	//Windowの位置を取得
	/*
	戻り値: {x, y}
	*/
	FluidumDrawAPI _NODISCARD std::pair<int32_t, int32_t> getWindowPos();


	//Windowのサイズを変更
	/*
	引数:
		width  横
		height 縦
	*/
	FluidumDrawAPI void setWindowSize(const int32_t width, const int32_t height);
	/*
	引数:
		size サイズ
	*/
	FluidumDrawAPI void setWindowSize(const ImVec2& size);


	//Windowの位置を変更
	/*
	引数:
		x 横
		y 縦
	*/
	FluidumDrawAPI void setWindowPos(const int32_t x, const int32_t y);
	/*
	引数:
		pos 位置
	*/
	FluidumDrawAPI void setWindowPos(const ImVec2& pos);


	//Windowのサイズと位置を変更
	/*
	引数:
		width  横のサイズ
		height 縦のサイズ
		x	   横の位置
		y	   縦の位置
	*/
	FluidumDrawAPI void setWindowSizePos(const int32_t width, const int32_t height, const int32_t x, const int32_t y);
	/*
	引数:
		size サイズ
		pos 位置
	*/
	FluidumDrawAPI void setWindowSizePos(const ImVec2& size, const ImVec2& pos);


	FluidumDrawAPI void miximizeWindow();

	FluidumDrawAPI void minimizeWindow();


}

//Image
namespace FDR {

	//ImGuiで使用できるImageを作成して取得する
	/*
	戻り値はImGuiImageで，内部にImTextureIDをもっている．
	ImGui::Image(ImTextureID,...)関数のオーバーロードとしてImGui::Image(FDR::ImGuiImage,...)を提供する．
	これは次のようにリソースを解放するため，ImGuiImage内部のImTextureID(void*)を取得すると無効なアドレスを指してしまう可能性があるからだ．
	内部でshared_ptrを使っている．リソースの解放は共有数が0になると解放される．

	引数:
		filePath 読み込む画像のパス(.png .jpeg .jpg .ppm ...)

	*/
	FluidumDrawAPI ImGuiImage createImGuiImage(const char* filePath);


}

//Font
namespace FDR {

	//デフォルトで用意したフォントの小さいサイズを取得 ImGui::PushFont用
	FluidumDrawAPI _NODISCARD ImFont* getDefaultFontMiniSize();


}