#pragma once

#include "../../Common/common.h"

namespace FS::Utils {

	class AddScenes final : public Scene {
	public:
		explicit AddScenes(
			const std::vector<FU::Class::ClassCode::CodeType>& codes
		);
		void Constructor();

		~AddScenes() noexcept;

		FluidumUtils_Class_Delete_CopyMove(AddScenes);

	public:
		virtual void call() override;

	private:
		const std::vector<FU::Class::ClassCode::CodeType> codes;

	};
}