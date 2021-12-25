#pragma once

#include "../../Common/common.h"
#include "shared_info.h"

namespace FS::Project::Add {

	class Directory final : public Scene {
	public:
		explicit Directory(
			const FD::GuiRead* const guiRead,
			const FD::SceneRead* const sceneRead,
			std::shared_ptr<SharedInfo>& sharedInfo
		);
		void Constructor(
			FD::GuiRead,
			FD::SceneRead
		);

		~Directory() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Directory)

	public:
		virtual void call() override;

	private://data
		const FD::SceneRead* const sceneRead;

		FD::Text::ProjectDirectory text{};

	private:
		std::shared_ptr<SharedInfo> sharedInfo;

		struct {
			std::string name{};
			std::string parent{};
		}str;

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			float innerPosHeight{};
		}style;

		struct {
			ImVec2 name{};
			ImVec2 create{};

		}pos;

	private:
		void title();

	private:
		void parent();

	private:
		void name();

	private:
		void bottom();
		void deleteThisScene();

	private:
		void create();
	
	};
}