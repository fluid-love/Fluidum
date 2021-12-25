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

		struct Supported final {
			//element(uint32_t) -> line number
			std::vector<uint32_t> breakpoints{};
		};

		struct Unsupported final {

		};

		enum class Type : uint8_t {
			Directory,
			Supported,
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
		std::vector<std::shared_ptr<Supported>> codes{};
		std::vector<std::shared_ptr<Unsupported>> unsupported{};

	public:
		Ref* add(const std::string& parent, const std::string& path, const Directory& info);
		Ref* add(const std::string& parent, const std::string& path, const Supported& info);
		Ref* add(const std::string& parent, const std::string& path, const Unsupported& info);

		//samePath() && exist -> fail
		//samePath() && !exist -> swap
		//!samePath() -> add 
		//fail -> return nullptr
		Ref* tryAdd(const std::string& parent, const std::string& path, const Directory& info);
		Ref* tryAdd(const std::string& parent, const std::string& path, const Supported& info);
		Ref* tryAdd(const std::string& parent, const std::string& path, const Unsupported& info);

		void erase(const std::string& path);

		void changePath(const std::string& path, const std::string& newPath);
		void changeName(const std::string& path, const std::string& newName);

		//C:/test/test test
		//---------------
		//path:    C:/test/hoge
		//name: hoge
		void changePathAndName(const std::string& path, const std::string& newName);

		void sync();
	private:
		void sync(std::vector<Ref>& info);
	public:
		//all
		[[nodiscard]] bool samePath(const std::string& path);

		//same directory
		[[nodiscard]] bool sameName(const std::string& path, const std::string& name);

	public:
		[[nodiscard]] std::vector<Ref>* get() noexcept;

	private:
		void forEach_recursive(void(*function)(Ref&), Ref& ref);
		void forEach_recursive(void(*function)(Ref&, void*), Ref& ref, void* userData);

	public:
		void forEach(void(*function)(Ref&));
		void forEach(void(*function)(Ref&, void*),void* userData);

	private:
		std::optional<RefIterator> find_recursive(RefIterator rec, const std::string& path);
		std::optional<RefIterator> find(const std::string& path);

		std::optional<RefIterator> find_parent_recursive(RefIterator rec, const std::string& path);
		std::optional<RefIterator> find_parent(const std::string& path);

		void deletePtr(RefIterator itr);

	private:
		bool tryReplaceExistElm(RefIterator itr, Ref& ref);
		Ref* addRef(const std::string& parent, const Ref& ref);

	private:
		Ref makeRef(const std::string& path, const Directory& info) const;
		Ref makeRef(const std::string& path, const Supported& info) const;
		Ref makeRef(const std::string& path, const Unsupported& info) const;

	};

	template<typename T>
	concept IsFileInfoElm =
		std::same_as<T, FileList::Directory> ||
		std::same_as<T, FileList::Supported> ||
		std::same_as<T, FileList::Unsupported>;


}

namespace FD::Project::FileList {
	using Type = Internal::FileList::Type;
	
	using FileInfo = Internal::FileList::Ref;

	using DirectoryInfo = Internal::FileList::Directory;
	using SupportedInfo = Internal::FileList::Supported;
	using UnsupportedInfo = Internal::FileList::Unsupported;

}

