#pragma once

#include "../../../Common/common.h"

namespace FS::Bar {

	class Exit final : public Scene {
	public:
		explicit Exit(
			const FD::WindowRead* const windowRead,
			FD::WindowWrite* const windowWrite,
			const FD::SceneRead* const sceneRead,
			const FD::ProjectRead* const projectRead,
			FD::ExitWrite* const exitWrite,
			const FD::Coding::TabRead* const tabRead,
			FD::Coding::TabWrite* const tabWrite
		);
		void Constructor(
			FD::WindowRead,
			FD::WindowWrite,
			FD::SceneRead,
			FD::ProjectRead,
			FD::ExitWrite,
			FD::Coding::TabRead,
			FD::Coding::TabWrite
		);

		~Exit() noexcept;

	public:
		virtual void call() override;

	private://data
		const FD::WindowRead* const windowRead;
		FD::WindowWrite* const windowWrite;
		const FD::SceneRead* const sceneRead;
		const FD::ProjectRead* const projectRead;
		FD::ExitWrite* const exitWrite;
		const FD::Coding::TabRead* const tabRead;
		FD::Coding::TabWrite* const tabWrite;

	private://data

		const FD::Text::BarExit text{};

	private:
		void exit();
		bool confirmDefaultProject();
		bool confirmTab();

	private:
		void deleteThis();
	};
}