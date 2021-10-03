#pragma once

#include "../../Common/common.h"

namespace FS::Internal::Coding {

	class Tab final : public Scene {
	public:
		explicit Tab(const FD::ProjectRead* const projectRead);
		void Constructor(FD::ProjectRead);

		~Tab() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::ProjectRead* const projectRead;

		bool windowCloseFlag = false;
	private:
		void checkWindowShouldClose();

		void popup();

		void buttons();

	};
}