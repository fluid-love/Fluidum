#pragma once

#include "Check/check.h"

#define Fluidum_Lua_Api

namespace FS::Lua {

	class Calc final : public Scene {
	public:
		explicit Calc(
			const FD::ProjectRead* const projectread,
			FD::ConsoleWrite* const consoleWrite,
			FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite,
			FD::ImPlotWrite* const implotWrite,
			const FD::ImPlotRead* const implotRead
		);
		void Constructor(
			FD::ProjectRead,
			FD::ConsoleWrite,
			FD::FunctionWrite<FD::Calc::Language::Lua>,
			FD::ImPlotWrite,
			FD::ImPlotRead
		);

		~Calc() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Calc)

	public:
		virtual void call() override;

	private:
		using Type = FD::Calc::Lua::FunctionType;
	private://data
		const FD::ProjectRead* const projectread;
		FD::ConsoleWrite* const consoleWrite;
		FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite;
		FD::ImPlotWrite* const implotWrite;
		const FD::ImPlotRead* const implotRead;

		Internal::Check check{ consoleWrite };

		//lua state
		lua_State* const state;

	private:
		void registerCppFunctions();//c++関数の登録
		void registerLuaLibraries();//luaライブラリの登録
		void terminate();

		/* ↓Lua Functions↓ */
	private://Genome

	//	//genomeの作成
	//	/*
	//	引数：
	//		個体数
	//		ゲノムサイズ
	//		ゲノムタイプ = bin
	//		初期化方法 = random
	//	*/
	//	FLUIDUM_LUA_API Ret create_Genome(State L);

	//	//一点交叉
	//	/*
	//	引数：
	//		親1の個体番号
	//		親2の個体番号
	//		交叉させる位置 1,2,3...
	//	*/
	//	FLUIDUM_LUA_API Ret singlePointCrossOver(State L);

	//	//反転
	//	/*
	//	引数：
	//		個体番号
	//		反転させる位置
	//	*/
	//	FLUIDUM_LUA_API Ret flip_Genome(State L);

	//	//0 1を入れる
	//	/*
	//	引数：
	//		個体番号
	//		位置
	//	*/
	//	FLUIDUM_LUA_API Ret set_Genome(State L);
	//	FLUIDUM_LUA_API Ret unset_Genome(State L);

	//	//個体を削除
	//	/*
	//	引数：
	//		個体番号
	//	*/
	//	FLUIDUM_LUA_API Ret erase_Genome(State L);

	//	//binaryから非負十進数の値に変換
	//	/*
	//	引数：
	//		個体番号
	//		サイズ
	//		オフセット
	//	*/
	//	FLUIDUM_LUA_API Ret toUInt_Genome(State L);

	//	//一時的なgenomeをプッシュする
	//	/*
	//	引数：
	//		一時的なgenomeのindex
	//	*/
	//	Fluidum_Lua_Api Ret push_Genome(State L);

	//private://Math
	//	Fluidum_Lua_Api Ret hypot(State L);//距離を返す
	//	Fluidum_Lua_Api Ret randomInt(State L);//ランダムな整数値を返す

	private://FPlot

		Fluidum_Lua_Api Ret plot_make(State L);
		Fluidum_Lua_Api Ret plot_pushBack(State L);
		Fluidum_Lua_Api Ret plot_setMarker(State L);
		Fluidum_Lua_Api Ret plot_makePlot(State L);
		Fluidum_Lua_Api Ret plot_plot(State L);


		//private://FDraw

		//	Fluidum_Lua_Api Ret create_Draw(State L);
		//	Fluidum_Lua_Api Ret updateVertex(State L);
		//	Fluidum_Lua_Api Ret updateIndex(State L);
		//	Fluidum_Lua_Api Ret clear_Draw(State L);

	private://FSystem

		Fluidum_Lua_Api Ret sleepSeconds(State L);
		Fluidum_Lua_Api Ret sleepMilliSeconds(State L);

		//終了させる
		FluidumUtils_Debug_BeginDisableWarning(4646)
			Fluidum_Lua_Api [[noreturn]] Ret terminate(State L);
		FluidumUtils_Debug_EndDisableWarning

			//private:
			//	FLUIDUM_LUA_API Ret create_Piano(State L);

	};


	namespace Internal {
		using MenberFunction = ::FS::Lua::Ret(Calc::*)(::FS::Lua::State L);
		template <MenberFunction Func>
		::FS::Lua::Ret dispatch(::FS::Lua::State L) {
			Calc* ptr = *static_cast<Calc**>(lua_getextraspace(L));
			return ((*ptr).*Func)(L);
		}
	}

}