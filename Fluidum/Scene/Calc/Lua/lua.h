#pragma once

#include "Common/check.h"

#ifndef Fluidum_Lua_Api
#define Fluidum_Lua_Api
#else
#error AlreadyDefined
#endif

namespace FS::Calc::Lua {

	class Run final : public Scene {
	public:
		explicit Run(
			const FD::ProjectRead* const projectRead,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			FD::ConsoleWrite* const consoleWrite,
			FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite,
			FD::ImPlotWrite* const implotWrite,
			const FD::ImPlotRead* const implotRead,
			FD::Calc::ArrayWrite* const arrayWrite,
			const FD::Calc::ArrayRead* const arrayRead
		);
		void Constructor(
			FD::ProjectRead,
			FD::FluidumFilesRead,
			FD::ConsoleWrite,
			FD::FunctionWrite<FD::Calc::Language::Lua>,
			FD::ImPlotWrite,
			FD::ImPlotRead,
			FD::Calc::ArrayWrite,
			FD::Calc::ArrayRead
		);

		~Run() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Run)

	public:
		virtual void call() override;

	private:
		using Type = FD::Calc::Lua::FunctionType;
		
		//log
		using Message = ::FS::Calc::Lua::Internal::Message;
		using LogType = ::FS::Calc::Lua::Internal::LogType;

	private:
		const FD::ProjectRead* const projectRead;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		FD::ConsoleWrite* const consoleWrite;
		FD::FunctionWrite<FD::Calc::Language::Lua>* const functionWrite;
		FD::ImPlotWrite* const implotWrite;
		const FD::ImPlotRead* const implotRead;
		FD::Calc::ArrayWrite* const arrayWrite;
		const FD::Calc::ArrayRead* const arrayRead;

		Internal::Check check{ consoleWrite };

		//lua state
		lua_State* const state;

	private:
		void registerCppFunctions();
		bool init();
		void terminate();

		/* ↓Lua Functions↓ */
	private://register cpp functions
		Fluidum_Lua_Api Ret _Fluidum_Register_CppFunctions_FSLPath(State L);

		Fluidum_Lua_Api Ret _Fluidum_Register_CppFunctions_System(State L);
		Fluidum_Lua_Api Ret _Fluidum_Register_CppFunctions_Array(State L);
		Fluidum_Lua_Api Ret _Fluidum_Register_CppFunctions_Plot(State L);

	private://system
		Fluidum_Lua_Api Ret system_print(State L);

	private://array
		Fluidum_Lua_Api Ret array_make(State L);


		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_empty(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_size(State L);


		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_front(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_back(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array___index(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array___newindex(State L);


		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_set(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_push_front(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_push_back(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_pop_front(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_pop_back(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_clear(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_erase(State L);

		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array_erase_if(State L);


		template<FD::Calc::Array::ValueType>
		Fluidum_Lua_Api Ret array___gc(State L);







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
		Fluidum_Lua_Api Ret plot_push_back(State L);
		Fluidum_Lua_Api Ret plot_marker(State L);
		Fluidum_Lua_Api Ret plot_make_plot(State L);
		Fluidum_Lua_Api Ret plot_plot(State L);
		Fluidum_Lua_Api Ret plot_display(State L);
		Fluidum_Lua_Api Ret plot_hide(State L);


	private://FDraw

		//	Fluidum_Lua_Api Ret create_Draw(State L);
		//	Fluidum_Lua_Api Ret updateVertex(State L);
		//	Fluidum_Lua_Api Ret updateIndex(State L);
		//	Fluidum_Lua_Api Ret clear_Draw(State L);

	private://system

		Fluidum_Lua_Api Ret system_sleep_seconds(State L);
		Fluidum_Lua_Api Ret system_sleep_milliseconds(State L);

		//throw Internal::Exception
		FluidumUtils_Debug_BeginDisableWarning(4646)
			Fluidum_Lua_Api [[noreturn]] Ret system_terminate(State L);
		FluidumUtils_Debug_EndDisableWarning

			//private:
			//	FLUIDUM_LUA_API Ret create_Piano(State L);

	};

	namespace Internal {
		using MenberFunction = ::FS::Calc::Lua::Ret(Run::*)(::FS::Calc::Lua::State L);
		template <MenberFunction Func>
		::FS::Calc::Lua::Ret dispatch(::FS::Calc::Lua::State L) {
			Run* ptr = *static_cast<Run**>(lua_getextraspace(L));
			return ((*ptr).*Func)(L);
		}
	}

}