#pragma once

#include "../../Common/common.h"

namespace FS::Calc {

	class Run final : public Scene {
	public:
		enum class InfoErrorType : UIF8 {
			NoError,
			NotSetProjectType,
			NotExistsEntryFile,
			InternalError
		};

		struct Info final {
			InfoErrorType errorType = InfoErrorType::NoError;
		};

	public:
		explicit Run(
			const FD::Project::PropertyRead* const propertyRead,
			const FD::Project::PropertyLuaRead* const propertyLuaRead,
			Info& info
		);
		void Constructor(
			FD::Project::PropertyRead,
			FD::Project::PropertyLuaRead
		);

		~Run() noexcept;

		FluidumUtils_Class_Delete_CopyMove(Run);

	public:
		virtual void call() override;

	private:
		const FD::Project::PropertyRead* const propertyRead;
		const FD::Project::PropertyLuaRead* const propertyLuaRead;

		Info& info;

	private:
		void run();

	};

}