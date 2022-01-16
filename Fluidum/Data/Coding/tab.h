#pragma once

#include "../Common/common.h"
#include "../../TextEditor/include.h"

//forward declaration
namespace FD {

	namespace Coding {
		class TabWrite;
		class TabRead;
		class DisplayWrite;
		class DisplayRead;
	}

	class ProjectWrite;

}

namespace FD::Coding {

	struct DisplayInfo final {
	public:
		enum class Theme : UT {
			Default,
			Dark,
			Light,
			Blue
		};

	public:
		std::string path{};
		float zoomRatio = 1.0f;
		Theme theme{};

	};

}

namespace FD::Coding::Internal {

	class Data final {
	private:
		static inline std::vector<std::string> filePaths{};

	private:
		static inline std::vector<DisplayInfo> displayInfo{};
		static inline float firstEditorZoomRatio = 1.0f;

	private:
		static inline std::mutex mtx{};
		static inline std::atomic_bool save = false;

	private:
		//To load project file.
		struct ProjectWrite final {
		private:
			static void initializeInternalData();
			static void clearInternalData();

		private:
			friend ::FD::ProjectWrite;

		};

	private:
		friend ::FD::Coding::TabWrite;
		friend ::FD::Coding::TabRead;
		friend ::FD::Coding::DisplayWrite;
		friend ::FD::Coding::DisplayRead;
		friend ::FD::ProjectWrite;
	};

}

namespace FD::Coding {

	class TabWrite final {
	public:
		explicit TabWrite(::FD::Internal::PassKey) noexcept {}
		~TabWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(TabWrite);

	public:
		enum class Exception : UT {
			LimitFileSizeMax,
			NotFound,
			AlreadyExist,
		};

	public:
		//LimitFileSizeMax == 1000
		//AlreadyExist
		//NotFound(file)
		void add(const std::string& path);

		//NotFound
		void remove(const std::string& path);

		void clear();

	public:
		void setIsTextSaved(const std::string& path, const bool val);
		void setAllIsTextSaved(const bool val);

	public:
		//NotFound
		void saveText(const std::string& path);

		void saveAllTexts();

		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		//Be careful with multi-threading.
		[[nodiscard]] FTE::TextEditor* getEditor(const std::string& path) const;

	public:
		void save();

	private:
		void update();

	};

}

namespace FD::Coding {

	class TabRead final {
	public:
		explicit TabRead(::FD::Internal::PassKey) noexcept {};
		~TabRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(TabRead);

	public:
		[[nodiscard]] bool update() const;
		[[nodiscard]] std::vector<std::string> paths() const;

	public:
		[[nodiscard]] bool update_isTextSaved() const;
		[[nodiscard]] bool isTextSaved(const std::string& path) const;

		//no-throw
		[[nodiscard]] bool isAllTextSaved() const noexcept;

		[[nodiscard]] std::vector<std::string> notSavedTexts() const;

	public:
		[[nodiscard]] bool exist(const std::string& path) const;

	};

}

namespace FD::Coding {

	class DisplayWrite final {
	public:
		explicit DisplayWrite(::FD::Internal::PassKey) noexcept {};
		~DisplayWrite() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(DisplayWrite);

	public:
		enum class Exception : UT {
			LimitFileSizeMax,
			NotFound,
			AlreadyExist,
		};

	public:
		//NotFound
		void add(const std::string& path);
		void remove(const std::string& path);

		bool tryRemove(const std::string& path);

		void theme(const std::string& path, const DisplayInfo::Theme theme);
		void zoomRatio(const std::string& path, const float ratio);

	public:
		/*
		Exception:
			std::exception
			NotFound
		*/
		//strong
		void focusedEditor(const std::string& path);

	private:
		std::vector<DisplayInfo>::iterator find(const std::string& path) const;

	};

}

namespace FD::Coding {

	class DisplayRead final {
	public:
		explicit DisplayRead(::FD::Internal::PassKey) noexcept {};
		~DisplayRead() noexcept = default;
		FluidumUtils_Class_Delete_CopyMove(DisplayRead);

	public:
		[[nodiscard]] bool changed() const;

	public:
		[[nodiscard]] std::vector<std::string> paths() const;
		[[nodiscard]] std::vector<DisplayInfo> info() const;

	public:
		//no-throw
		[[nodiscard]] bool empty() const noexcept;

		/*
		Exception:
			std::exception
		*/
		//strong
		[[nodiscard]] bool isEditorFocused(const std::string& path) const;
		[[nodiscard]] bool isEditorFocused(const FTE::TextEditor* editor) const noexcept;

		/*
		Exception:
			std::exception
		*/
		//strong
		//Returns an empty string if no editor is focused.
		[[nodiscard]] std::string focusedFilePath() const;


	};

}