#pragma once

#include "../Common/common.h"
#include "../../TextEditor/include.h"
#include "shared_info.h"

namespace FS::File::Add {

	class New final : public Scene {
	public:
		explicit New(
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			const FD::History::FileRead* const fileRead,
			std::shared_ptr<SharedInfo>& sharedInfo
		);
		void Constructor(
			FD::ProjectRead,
			FD::GuiRead,
			FD::History::FileRead
		);

		~New() noexcept;

		FluidumUtils_Class_Delete_CopyMove(New);

	public:
		virtual void call() override;

	private:
		const FD::ProjectRead* const projectRead;

		FD::Text::ProjectNewFile text{};

	private:
		std::shared_ptr<SharedInfo> sharedInfo;

		struct {
			std::string searchStr{};

			std::string directoryPath{};
			std::string fileName{};
			std::string extension{};
			std::string fullPath{};
		}str;

		bool only = false;

		struct {
			ImCounter<ImAnimeTime> counter{};
			ImCounter<ImAnimeTime> form{};
		}anime;

		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			ImVec2 buttonSize{};

			float innerPosHeight = 0.0f;
		}style;

		struct {
			ImVec2 directoryPath{};
			ImVec2 fileName{};
			ImVec2 create{};
		}pos;

		struct ButtonInfo final {
			const FDR::ImGuiImage& image;
			const char* label;
			const std::string& title;
			const char* description;
			const char* extension;
			FD::History::File::Type type{};

			bool hide = false;
		};


		const std::vector<ButtonInfo> recentButtons;
		const std::vector<FDR::ImGuiImage> images;

		const std::vector<ButtonInfo> emptyFiles;

		struct {
			const ButtonInfo* ptr = nullptr;
		}select;

	private:
		void title();

	private:
		void search();

	private:
		void recent();

	private:
		void list();
		void filter();
		void buttons();

	private:
		void form();
		void directoryPath();
		void fileName();

	private:
		void bottom();
		bool check();
		void tryCreate();
		bool create();

	private:
		//return [left, right]
		std::pair<bool, bool> button(
			const ButtonInfo* const info,
			const std::optional<float> width = std::nullopt
		);
	private:
		std::vector<ButtonInfo> initRecentFileTypes(const std::vector<FD::History::File::Type>& types);
		void deleteThisScene();

	};

}