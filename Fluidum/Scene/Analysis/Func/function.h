#pragma once

#include "../../Common/common.h"

namespace FS::Analysis {

	class Function final : public Scene {
	public:
		template<FD::Calc::Language Lang>
		explicit Function(const FD::FunctionRead<Lang>* const functionRead) {

			this->functionRead<Lang> = functionRead;
		}

		void Constructor(FD::FunctionRead<FD::Calc::Language::Lua>);

		~Function() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Function)

	public:
		virtual void call() override;

	private:
		template<FD::Calc::Language Lang>
		static inline const FD::FunctionRead<Lang>* functionRead = nullptr;

	private:
		void table();

	};
}