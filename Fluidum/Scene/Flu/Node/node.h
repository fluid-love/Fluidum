#pragma once

#include "../../Common/common.h"

namespace FS::Flu {

	class Node final : public Scene {
	public:
		explicit Node();
		void Constructor();

		~Node() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Node)

	public:
		virtual void call() override;

	private://data


	private:

	
	private:

	};
}