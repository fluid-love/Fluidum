#pragma once

#include "../Common/common.h"

namespace FS::File::Add {

	class CheckPath final : public Scene {
	public:
		struct Info final {
		public://return
			bool noerror{};
			std::string fullPath{};

		public://arga
			bool project{};
			bool directory{};

			const std::string& parent;
			const std::string& name;

			const ImVec2& pos_name;
			const ImVec2& pos_path;
			const ImVec2& pos_create;
		};

	public:
		explicit CheckPath(
			const FD::ProjectRead* const projectRead,
			const FD::UserFilesRead* const userFilesRead,
			Info& info
		);
		void Constructor(
			FD::ProjectRead,
			FD::UserFilesRead
		);

		~CheckPath() noexcept;

		FluidumUtils_Class_Delete_CopyMove(CheckPath)

	public:
		virtual void call() override;

	private://data
		const FD::ProjectRead* const projectRead;
		const FD::UserFilesRead* const userFilesRead;
		Info& info;

		FD::Text::ProjectCheckPath text{};

	private:
		void project();
		void user();
		bool user_directory(const std::string& path);
		bool user_file(const std::string& path);

	};

}