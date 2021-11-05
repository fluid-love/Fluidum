#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS::Coding {

	class New final : public Scene {
	public:
		explicit New(
			FD::Coding::TabWrite* const tabWrite,
			const FD::ProjectRead* const projectRead,
			FD::ProjectWrite* const projectWrite,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			FD::FluidumFilesWrite* const fluidumFilesWrite,
			const FD::GuiRead* const guiRead,
			const FD::Log::FileRead* const fileRead
		);
		void Constructor(
			FD::Coding::TabWrite,
			FD::ProjectRead,
			FD::ProjectWrite,
			FD::FluidumFilesRead,
			FD::FluidumFilesWrite,
			FD::GuiRead,
			FD::Log::FileRead
		);

		~New() noexcept;

		FluidumUtils_Class_Delete_CopyMove(New)

	public:
		virtual void call() override;

	private://data
		FD::Coding::TabWrite* const tabWrite;
		const FD::ProjectRead* const projectRead;
		FD::ProjectWrite* const projectWrite;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		FD::FluidumFilesWrite* const fluidumFilesWrite;

		FD::Text::CodingNew text{};

	private:
		const std::string projectType;

		std::string folderPath{};
		std::string fileName{};
		std::string extension{};
		std::string fullPath{};

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			ImVec2 buttonSize{};

			float innerPosHeight = 0.0f;
		}style;

		struct {
			ImVec2 folderPath{};
			ImVec2 fileName{};
			ImVec2 create{};
		}pos;

		struct ButtonInfo final {
			FDR::ImGuiImage image;
			const char* label;
			const char* title;
			const char* description;
		};

		const std::vector<ButtonInfo> recentButtons;
		const std::vector<FDR::ImGuiImage> images;

		const std::vector<ButtonInfo> emptyFiles;

		struct {
			ButtonInfo* ptr = nullptr;
			FD::Log::File::Type type = FD::Log::File::Type::None;
		}select;

	private:
		void title();

	private:
		void recent();

	private:
		void list();
		void filter();
		void buttons();

	private:
		void form();
		void bottom();
		bool check();
		void create();

	private:
		bool button(const FDR::ImGuiImage& image, const char* label, const char* title, const char* description);

	private:
		std::vector<ButtonInfo> initRecentFileTypes(const std::vector<FD::Log::File::Type>& types);

	};
}