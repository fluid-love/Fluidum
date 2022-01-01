#pragma once

#include "../Common/common.h"
#include "../../TextEditor/include.h"

//forward
namespace FD {
	namespace Coding {
		class TabWrite;
		class TabRead;
		class DisplayWrite;
		class DisplayRead;
	}
	class ProjectWrite;
}

namespace FD::Coding::Internal {

	class Data final {
	private:
		static inline std::vector<std::string> filePaths{};
		static inline std::vector<std::string> displayFiles{};
		static inline std::mutex mtx{};

		static inline std::atomic_bool save = false;

	private:
		friend ::FD::Coding::TabWrite;
		friend ::FD::Coding::TabRead;
		friend ::FD::Coding::DisplayWrite;
		friend ::FD::Coding::DisplayRead;
		friend ::FD::ProjectWrite;
	};


}


namespace FD::Coding {

	//tab
	class TabWrite final {
	public:
		explicit TabWrite(::FD::Internal::PassKey) {}
		~TabWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(TabWrite)

	public:
		enum class Exception : uint8_t {
			LimitFileSizeMax,
			NotFound,
			AlreadyExist,
		};

	public:
		//LimitFileSizeMax == 1000
		//AlreadyExist
		//NotFound(file)
		void add(const std::string& path) const;

		//NotFound
		void remove(const std::string& path) const;
	
		void clear() const;

	public:
		void setIsTextSaved(const std::string& path, const bool val) const;
		void setAllIsTextSaved(const bool val) const;
		void releaseAllEditor() const;

	public:
		//NotFound
		void saveText(const std::string& path) const;

		void saveAllTexts() const;

		//NotFound
		[[nodiscard]] FTE::TextEditor* getEditor(const std::string& path) const;

	public:
		void save() const;

	private:
		void update() const;

	};

	//current project
	class TabRead final {
	public:
		explicit TabRead(::FD::Internal::PassKey) noexcept {};
		~TabRead() = default;
		FluidumUtils_Class_Delete_CopyMove(TabRead)

	public:

		[[nodiscard]] bool update() const;
		[[nodiscard]] std::vector<std::string> paths() const;

	public:
		[[nodiscard]] bool update_isTextSaved() const;
		[[nodiscard]] bool isTextSaved(const std::string& path) const;
		[[nodiscard]] bool isAllTextSaved() const;

		[[nodiscard]] std::vector<std::string> notSavedTexts() const;

	public:
		[[nodiscard]] bool exist(const std::string& path) const;


	};

}

namespace FD::Coding {

	class DisplayWrite final {
	public:
		explicit DisplayWrite(::FD::Internal::PassKey) noexcept {};
		~DisplayWrite() = default;
		FluidumUtils_Class_Delete_CopyMove(DisplayWrite)

	public:
		enum class Exception : uint8_t {
			LimitFileSizeMax,
			NotFound,
			AlreadyExist,
		};

	public:
		//NotFound
		void add(const std::string& path) const;
		void remove(const std::string& path) const;

		bool tryRemove(const std::string& path) const;
	};

	class DisplayRead final {
	public:
		explicit DisplayRead(::FD::Internal::PassKey) noexcept {};
		~DisplayRead() = default;
		FluidumUtils_Class_Delete_CopyMove(DisplayRead)

	public:
		[[nodiscard]] bool changed() const;

	public:
		[[nodiscard]] std::vector<std::string> paths() const;
		[[nodiscard]] bool empty() const;


	};
}