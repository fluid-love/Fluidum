#pragma once

#include "Check/check.h"

#define Fluidum_Lua_Api

namespace FS {

	class LuaCalc final : public Scene {
	public:
		explicit LuaCalc(const FD::ProjectRead* const projectread);
		void Constructor(FD::ProjectRead);

		~LuaCalc() noexcept;

	public:
		virtual void call() override;

	private://using
		using Ret = Lua::Ret;
		using State = Lua::State;

	private://data
		const FD::ProjectRead* const projectread;

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
	//	FLUIDUM_LUA_API Ret push_Genome(State L);

	//private://Math
	//	FLUIDUM_LUA_API Ret hypot(State L);//距離を返す
	//	FLUIDUM_LUA_API Ret randomInt(State L);//ランダムな整数値を返す

	//private://FPlot

	//	//plotのベースを作成してfigureを埋める．そしてそのplotの番号を返す．
	//	FLUIDUM_LUA_API Ret create_Plot(State L);

	//	FLUIDUM_LUA_API Ret push_Plot(State L);

	//	FLUIDUM_LUA_API Ret marker_Plot(State L);

	//private://FDraw

	//	FLUIDUM_LUA_API Ret create_Draw(State L);
	//	FLUIDUM_LUA_API Ret updateVertex(State L);
	//	FLUIDUM_LUA_API Ret updateIndex(State L);
	//	FLUIDUM_LUA_API Ret clear_Draw(State L);



	private://FSystem

		Fluidum_Lua_Api Ret sleepSeconds(State L);
		Fluidum_Lua_Api Ret sleepMilliSeconds(State L);

		//終了させる
		Fluidum_Lua_Api [[noreturn]] Ret terminate(State L);

		//private:
		//	FLUIDUM_LUA_API Ret create_Piano(State L);



	};


	namespace Lua::Internal {
		using MenberFunction = ::FS::Lua::Ret(LuaCalc::*)(::FS::Lua::State L);
		template <MenberFunction Func>
		::FS::Lua::Ret dispatch(::FS::Lua::State L) {
			LuaCalc* ptr = *static_cast<LuaCalc**>(lua_getextraspace(L));
			return ((*ptr).*Func)(L);
		}
	}

}