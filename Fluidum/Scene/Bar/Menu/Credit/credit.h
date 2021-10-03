#pragma once

#include "../../../Common/common.h"

namespace FS::Internal::Bar {

	class Credit final : public Scene {
	public:
		explicit Credit();
		void Constructor();

		~Credit() noexcept;

	public:
		virtual void call() override;


	};
}