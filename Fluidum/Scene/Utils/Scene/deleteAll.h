#pragma once

#include "../../Common/common.h"

namespace FS::Utils {

	class DeleteAllScenes final : public Scene {
	public:
		explicit DeleteAllScenes();
		void Constructor();

		~DeleteAllScenes() noexcept;

		FluidumUtils_Class_Delete_CopyMove(DeleteAllScenes);

	public:
		virtual void call() override;

	private:
		void deleteAll();
	};

}