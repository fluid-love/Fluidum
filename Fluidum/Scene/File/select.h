#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"
#include "new.h"
#include "directory.h"

namespace FS::Project::Add {

	class Select final : public Scene {
	public:
		explicit Select(
			const FD::ProjectRead* const projectRead,
			const FD::GuiRead* const guiRead,
			const FD::SceneRead* const sceneRead,
			std::shared_ptr<SharedInfo>& sharedInfo 
		);
		void Constructor(
			FD::ProjectRead,
			FD::GuiRead,
			FD::SceneRead
		);

		~Select() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Select)

	public:
		virtual void call() override;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::SceneRead* const sceneRead;
		std::shared_ptr<SharedInfo> sharedInfo;

		FD::Text::ProjectSelect text{};

	private:

		const FDR::ImGuiImage newImage;
		const FDR::ImGuiImage openImage;
		const FDR::ImGuiImage dirImage;


		struct {
			ImCounter<ImAnimeTime> counter{};
			ImCounter<ImAnimeTime> form{};
		}anime;

		struct {
			ImVec2 windowSize{};
			ImVec2 windowPos{};

			bool isOpenWindowHovered = false;
			bool isNewFileWindowHovered = false;
			bool isDirWinfowHovered = false;

			bool project{};
		}style;

		struct {
			ImVec2 directoryPath{};
			ImVec2 fileName{};
			ImVec2 create{};
		}pos;

		enum class Template : uint8_t {
			None,
			Empty_Lua,
			Empty_Py,
			Empty_Cpp
		};

		Template selectType = Template::None;

		FTE::TextEditor textEditor{};
		bool isFileSelected = false;//color—p


		struct {
			std::string directoryPath{};
			std::string fileName{};
			std::string extension{};
			std::string fullPath{};
		}quickInfo;


	private:
		void title();

	private://recent
		void quick();
		void selectTemplate(const ImVec2& size);

	private://right
		void right();
		//return hovered ,left_button_clicked
		std::pair<bool, bool> button(
			const char* label,
			const char* description,
			const FDR::ImGuiImage& image,
			const bool hovered,
			const ImVec2& size
		);

	private://open
		void open(const ImVec2& size);
		void openDialog();

	private://newfile
		void newFile(const ImVec2& size);

	private://dir
		void dir(const ImVec2& size);

	private://lower right
		void bottomRight();

	private://bottom
		void bottom();

	private:
		void setEmptyFile();


		bool createNewFileQuick();
		[[nodiscard]] bool checkQuickInfo();

		void errorPopup();

	private:
		void close();

	};
}