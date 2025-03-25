#pragma once

#include "../../Common/common.h"

namespace FS::Analysis {

	class Overview final : public Scene {
	public:
		template<FD::Calc::Language Lang>
		explicit Overview(
			const FD::FunctionRead<Lang>* const functionRead,
			FD::ImGuiWindowWrite* const imguiWindowWrite,
			FD::ToolBarWrite* const toolBarWrite,
			const FD::ImPlotRead* const implotRead
		) : 
			imguiWindowWrite(imguiWindowWrite), 
			toolBarWrite(toolBarWrite),
			implotRead(implotRead)
		{
			FluidumScene_Log_Constructor(::FS::Analysis::Overview);

			this->functionRead<Lang> = functionRead;

			toolBarWrite->add(&Overview::topBar, this, "AnaOV");
		}

		void Constructor(
			FD::FunctionRead<FD::Calc::Language::Lua>, 
			FD::ImGuiWindowWrite, 
			FD::ToolBarWrite,
			FD::ImPlotRead
		);

		~Overview() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Overview);

	public:
		virtual void call() override;

	private:
		template<FD::Calc::Language Lang>
		static inline const FD::FunctionRead<Lang>* functionRead = nullptr;

		FD::ImGuiWindowWrite* const imguiWindowWrite;
		FD::ToolBarWrite* const toolBarWrite;
		const FD::ImPlotRead* const implotRead;

		bool windowShouldClose = true;


	private:
		void closeWindow();

	private:
		void setWindowInfo();

	private:
		void tab();

	private:
		void empty();

	private:
		void plot();

	private:
		void function();

	private:
		void topBar();

	};
}