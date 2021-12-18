#pragma once

#include "../../Common/common.h"
#include "../../../TextEditor/include.h"

namespace FS::Coding {

	class Select final : public Scene {
	public:
		explicit Select(
			FD::Coding::TabWrite* const tabWrite,
			const FD::ProjectRead* const projectRead,
			FD::ProjectWrite* const projectWrite,
			const FD::FluidumFilesRead* const fluidumFilesRead,
			FD::FluidumFilesWrite* const fluidumFilesWrite,
			const FD::GuiRead* const guiRead,
			const FD::SceneRead* const sceneRead
		);
		void Constructor(
			FD::Coding::TabWrite,
			FD::ProjectRead,
			FD::ProjectWrite,
			FD::FluidumFilesRead,
			FD::FluidumFilesWrite,
			FD::GuiRead,
			FD::SceneRead
		);

		~Select() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Select)

	public:
		virtual void call() override;

	private://data
		FD::Coding::TabWrite* const tabWrite;
		const FD::ProjectRead* const projectRead;
		FD::ProjectWrite* const projectWrite;
		const FD::FluidumFilesRead* const fluidumFilesRead;
		FD::FluidumFilesWrite* const fluidumFilesWrite;
		const FD::SceneRead* const sceneRead;

		FD::Text::CodingSelect text{};

	private:

		const FDR::ImGuiImage newImage;
		const FDR::ImGuiImage openImage;

		struct {
			ImCounter<ImAnimeTime> counter{};
		}anime;

		struct {
			ImVec2 windowSize{};
			ImVec2 windowPos{};

			bool isOpenWindowHovered = false;
			bool isNewFileWindowHovered = false;

		}style;

		struct {
			ImVec2 folderPath{};
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
			std::string folderPath{};
			std::string fileName{};
			std::string extension{};
			std::string fullPath{};
		}quickInfo;


	private:
		//make
		void create();

	private://recent
		void quick();
		void selectTemplate(const ImVec2& size);

	private://open
		void right();
		void open(const ImVec2& size);
		void openDialog();
		void newFile(const ImVec2& size);

	private://lower right
		void bottomRight();

	private://bottom
		void bottom();

	private:
		void setEmptyFile();


		void createNewFileQuick();
		[[nodiscard]] bool checkQuickInfo();

		void errorPopup();

	private:
		void close();

	private:
		struct Check final {
			enum class ErrorType : uint8_t {
				None,
				EmptyFileName,
				EmptyFolderPath,
				AlreadyExist,
				NotFound
			};
			static std::pair<ErrorType, std::string> checkFile(const std::string& folderPath, const std::string& fileName, const std::string& extension);
		};
		static int inputTextCallback(ImGuiInputTextCallbackData* data);
	};
}