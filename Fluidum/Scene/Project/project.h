#pragma once

#include "../Common/common.h"

namespace FS {

	class Project final : public Scene {
	public:
		explicit Project(
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			const FD::SceneRead* const sceneRead
		);
		void Constructor(
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::SceneRead
		);

		~Project() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Project)

	public:
		virtual void call() override;

	private:
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		const FD::SceneRead* const sceneRead;

		FD::Text::Project text{};

	private:
		struct {
			float topBarHeight{};
		}style;

	private:
		void topBar();

		void tab_sync();
		void tab_displayCode();

	};

}