#pragma once

#include "../../Common/common.h"

namespace FS::Project {

	class Property final : public Scene {
	public:
		explicit Property(
			const FD::GuiRead* const guiRead,
			const FD::Style::ColorRead* const colorRead,
			const FD::Style::VarRead* const varRead,
			FD::Project::PropertyWrite* const propertyWrite,
			const FD::Project::PropertyRead* const propertyRead,
			FD::Project::PropertyLuaWrite* const propertyLuaWrite,
			const FD::Project::PropertyLuaRead* const propertyLuaRead,
			const FD::ProjectFilesRead* const projectFilesRead,
			const FD::UserFilesRead* const userFilesRead
		);
		void Constructor(
			FD::GuiRead,
			FD::Style::ColorRead,
			FD::Style::VarRead,
			FD::Project::PropertyWrite,
			FD::Project::PropertyRead,
			FD::Project::PropertyLuaWrite,
			FD::Project::PropertyLuaRead,
			FD::ProjectFilesRead,
			FD::UserFilesRead
		);

		~Property() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Property);

	public:
		virtual void call() override;

	private:
		const FD::Style::VarRead* const varRead;
		const FD::Style::ColorRead* const colorRead;
		FD::Project::PropertyWrite* const propertyWrite;
		const FD::Project::PropertyRead* const propertyRead;
		FD::Project::PropertyLuaWrite* const propertyLuaWrite;
		const FD::Project::PropertyLuaRead* const propertyLuaRead;
		const FD::ProjectFilesRead* const projectFilesRead;
		const FD::UserFilesRead* const userFilesRead;

		FD::Text::ProjectProperty text{};

	private:
		struct {
			ImVec2 windowPos{};
			ImVec2 windowSize{};

			float tabHeight{};

			ImVec2 leftWindowSize{};
			ImVec2 rightWindowSize{};
			ImVec2 informationWindowSize{};

			float bottomHeight{};

			float infomationAreaHeight{};

			float windowWidth{};
			float innerHeight{};
			float dummy{};

		}style;

		struct {
			bool window = true;
		}flag;

		struct {
			ImCounter<ImAnimeTime> window;
		}anime;

		struct {
			FD::Project::Property::ProjectType current{};

			bool changeFlag = false;
			FD::Project::Property::ProjectType change{};

			[[nodiscard]] inline bool isLua() const noexcept {
				return current == FD::Project::Property::ProjectType::Lua;
			}
			[[nodiscard]] inline bool isPython() const noexcept {
				return current == FD::Project::Property::ProjectType::Python;
			}
			[[nodiscard]] inline bool isCpp() const noexcept {
				return current == FD::Project::Property::ProjectType::Cpp;
			}
			[[nodiscard]] inline bool isNone() const noexcept {
				return current == FD::Project::Property::ProjectType::None;
			}

			bool lua = false;
			bool python = false;
			bool cpp = false;
		}type;

		struct {
			const char* title = nullptr;
			const char* text = nullptr;
		}info;

		enum class Tab : UIF8 {
			Main,
			Language,
			Fluidum
		};

		enum class List : UIF16 {
			None,
			CurrentProjectType,
			EntryFilePath,
			Version
		};

		struct {
			Tab tab{};

			List list{};
			std::function<void()> listFunction{};
		}select;

		struct {
			std::string entryFilePath;

		}temp;

		struct {
			std::vector<List> changed{};

			[[nodiscard]] inline bool isPropertyChanged() const noexcept {
				return !changed.empty();
			}

			inline void clear() noexcept {
				changed.clear();
			}

			inline bool tryPush(const List type) {
				const auto find = std::find(changed.cbegin(), changed.cend(), type);
				if (find == changed.cend()) { //not exists
					changed.emplace_back(List::EntryFilePath);
					return true;
				}
				return false;
			}

		}save;

		struct Node final {
			const char* label = nullptr;
			const char* infoText = nullptr;
			void(Property::* function)() = nullptr;
			List list{};
		};

		struct {
			std::vector<Node> none_main{};

			std::vector<Node> lua_main{};
			std::vector<Node> lua_language{};
		}node;

	private://window
		void closeWindow();

	private://flag
		void catchProjectType();

	private:
		void tab();

	private://left
		void left();
		void leftLoop(const std::vector<Node>& nodes);
		void left_none();
		void left_lua();

	private://right
		void right();
		void right_currentProjectType();
		bool changeProjectType(const FD::Project::Property::ProjectType type);
		void right_entryFilePath();
		void right_luaVersion();

	private:
		void information();

	private://bottom
		void bottom();

	private://save
		void saveAllChanges();
		bool confirmNotSaved();
		void save_entryFilePath();

	private:
		void dummy();
		void dummyComboArrowButton(const float textWidth);

	private:
		std::vector<Node> makeNode_none_main() const;

		std::vector<Node> makeNode_lua_main() const;
		std::vector<Node> makeNode_lua_language() const;

		void init();

	};

}