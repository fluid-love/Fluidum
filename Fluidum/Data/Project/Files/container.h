#pragma once

#include "../../Common/common.h"

namespace FD::Project::Internal {
	class FileList final {
	public:
		FluidumUtils_Class_Default_ConDestructor(FileList)
			FluidumUtils_Class_Default_CopyMove(FileList)

	public:
		struct Directory final {

		};

		struct Code final {
			std::vector<uint32_t> breakpoints{};
		};

		struct Unsupported final {

		};

		enum class Type : uint8_t {
			Directory,
			Code,
			Unsupported,
		};
	public:
		struct Ref final {
			std::vector<Ref> dir_internal{};

			Type type{};

			std::string path{};
			std::string name{};
			bool open = false;
			bool exist = true;
			void* ptr = nullptr;
		};

	private:
		std::vector<Ref> refs{};
		using RefIterator = decltype(refs)::iterator;

	private:
		std::vector<std::shared_ptr<Directory>> directories{};
		std::vector<std::shared_ptr<Code>> codes{};
		std::vector<std::shared_ptr<Unsupported>> unsupported{};

	public:
		Ref* add(const std::string& parent, const std::string& path, const Directory& info);
		Ref* add(const std::string& parent, const std::string& path, const Code& info);
		Ref* add(const std::string& parent, const std::string& path, const Unsupported& info);

		void erase(const std::string& path);

		void changePath(const std::string& path, const std::string& newPath);
		void changeName(const std::string& path, const std::string& newName);

		//C:/test/test test
		//---------------
		//path:    C:/test/hoge
		//name: hoge
		void changePathAndName(const std::string& path, const std::string& newName);
	
	public:
		//all
		_NODISCARD bool samePath(const std::string& path);

		//same directory
		_NODISCARD bool sameName(const std::string& path, const std::string& name);

	public:
		_NODISCARD std::vector<Ref>* get() noexcept;

	private:
		std::optional<RefIterator> find_recursive(RefIterator rec, const std::string& path);
		std::optional<RefIterator> find(const std::string& path);

		std::optional<RefIterator> find_parent_recursive(RefIterator rec, const std::string& path);
		std::optional<RefIterator> find_parent(const std::string& path);

	};

	template<typename T>
	concept IsFileInfoElm =
		std::same_as<T, FileList::Directory> ||
		std::same_as<T, FileList::Code> ||
		std::same_as<T, FileList::Unsupported>;


}

namespace FD::Project::List {
	using Type = Internal::FileList::Type;
	using FileInfo = Internal::FileList::Ref;
	using DirectoryInfo = Internal::FileList::Directory;
	using CodeInfo = Internal::FileList::Code;
	using UnsupportedInfo = Internal::FileList::Unsupported;

}