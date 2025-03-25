#pragma once

#include "../../../Common/common.h"

namespace FS::Bar {

	class ProjectInfo final : public Scene {
	public:
		explicit ProjectInfo(
			FD::ImGuiWindowWrite* const imguiWindowWrite,
			const FD::Style::ColorRead* const colorRead,
			FD::ProjectWrite* const projectWrite,
			const FD::ProjectRead* const projectRead,
			FD::Project::PropertyWrite* const propertyWrite,
			const FD::Project::PropertyRead* const propertyRead,
			FD::Project::PropertyLuaWrite* const propertyLuaWrite,
			const FD::Project::PropertyLuaRead* const propertyLuaRead,
			FD::ToolBarWrite* const toolBarWrite,
			const FD::Style::VarRead* const varRead
		);
		void Constructor(
			FD::ImGuiWindowWrite,
			FD::Style::ColorRead,
			FD::ProjectWrite,
			FD::ProjectRead,
			FD::Project::PropertyWrite,
			FD::Project::PropertyRead,
			FD::Project::PropertyLuaWrite,
			FD::Project::PropertyLuaRead,
			FD::ToolBarWrite,
			FD::Style::VarRead
		);

		~ProjectInfo() noexcept;

		FluidumUtils_Class_Delete_CopyMove(ProjectInfo);

	public:
		virtual void call() override;

	private:
		FD::ImGuiWindowWrite* const imguiWindowWrite;
		const FD::Style::ColorRead* const colorRead;
		FD::ProjectWrite* const projectWrite;
		const FD::ProjectRead* const projectRead;
		FD::Project::PropertyWrite* const propertyWrite;
		const FD::Project::PropertyRead* const propertyRead;
		FD::Project::PropertyLuaWrite* const propertyLuaWrite;
		const FD::Project::PropertyLuaRead* const propertyLuaRead;
		FD::ToolBarWrite* const toolBarWrite;
		const FD::Style::VarRead* const varRead;

		FD::Text::Project text{};

	private:
		struct {
			bool window = true;
		}flag;

	private://misc
		void catchWindowEvents();
		void toolBar();
		void setImGuiWindow();

	private:
		void left();

	private:
		void overview();

	};

}