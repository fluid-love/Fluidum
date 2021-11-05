#include "container.h"

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::add(const std::string& parent, const std::string& path, const Directory& info) {
	directories.emplace_back(std::make_shared<Directory>(info));

	Ref ref;
	ref.type = Type::Directory;
	ref.path = path;
	ref.name = FU::File::directoryName(path);
	ref.ptr = directories.back().get();

	if (parent.empty()) {
		refs.emplace_back(ref);
		return &refs.back();
	}
	else {
		//find parent
		auto itr = this->find(parent);
		assert(itr != std::nullopt);
		return &itr.value()->dir_internal.emplace_back(ref);
	}
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::add(const std::string& parent, const std::string& path, const Code& info) {
	codes.emplace_back(std::make_shared<Code>(info));

	Ref ref;
	ref.type = Type::Code;
	ref.path = path;
	ref.name = FU::File::fileName(path);
	ref.ptr = codes.back().get();

	if (parent.empty()) {
		refs.emplace_back(ref);
		return &refs.back();
	}
	else {
		//find parent
		auto itr = this->find(parent);
		assert(itr != std::nullopt);
		return &itr.value()->dir_internal.emplace_back(ref);
	}
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::add(const std::string& parent, const std::string& path, const Unsupported& info) {
	unsupported.emplace_back(std::make_shared<Unsupported>(info));

	Ref ref;
	ref.type = Type::Unsupported;
	ref.path = path;
	ref.name = FU::File::fileName(path);
	ref.ptr = unsupported.back().get();

	if (parent.empty()) {
		refs.emplace_back(ref);
		return &refs.back();
	}
	else {
		//find parent
		auto itr = this->find(parent);
		assert(itr != std::nullopt);
		return &itr.value()->dir_internal.emplace_back(ref);
	}
}

void FD::Project::Internal::FileList::erase(const std::string& path) {
	auto itr = this->find(path);
	auto parentItr = this->find_parent(path);

	assert(itr != std::nullopt && parentItr != std::nullopt);

	parentItr.value()->dir_internal.erase(itr.value());
}

void FD::Project::Internal::FileList::changePath(const std::string& path, const std::string& newPath) {
	auto itr = this->find(path);
	if (itr == std::nullopt)
		throw std::runtime_error("Failed to change path.");
	itr.value()->path = newPath;
}

void FD::Project::Internal::FileList::changeName(const std::string& path, const std::string& newName) {
	auto itr = this->find(path);
	if (itr == std::nullopt)
		throw std::runtime_error("Failed to change name.");
	itr.value()->name = newName;
}

void FD::Project::Internal::FileList::changePathAndName(const std::string& path, const std::string& newName) {
	auto itr = this->find(path);
	if (itr == std::nullopt)
		throw std::runtime_error("Failed to change path and name.");

	std::string newPath = FU::File::directory(itr.value()->path) + newName;
	itr.value()->path = std::move(newPath);
	itr.value()->name = newName;
}

std::optional<FD::Project::Internal::FileList::RefIterator> FD::Project::Internal::FileList::find_recursive(RefIterator rec, const std::string& path) {

	for (auto itr = rec->dir_internal.begin(), end = rec->dir_internal.end(); itr != end; itr++) {
		if (itr->path == path) {
			return itr;
		}
		auto res = this->find_recursive(itr, path);
		if (res != std::nullopt)
			return res;
	}
	return std::nullopt;
}

std::optional<FD::Project::Internal::FileList::RefIterator> FD::Project::Internal::FileList::find(const std::string& path) {

	for (auto itr = refs.begin(), end = refs.end(); itr != end; itr++) {
		if (itr->path == path) {
			return itr;
		}
		auto rec = this->find_recursive(itr, path);
		if (rec != std::nullopt)
			return rec.value();
	}
	return std::nullopt;
}

std::optional<FD::Project::Internal::FileList::RefIterator> FD::Project::Internal::FileList::find_parent_recursive(RefIterator rec, const std::string& path) {
	for (auto itr = rec->dir_internal.begin(), end = rec->dir_internal.end(); itr != end; itr++) {
		if (itr->path == path) {
			return rec;
		}
		auto res = this->find_parent_recursive(itr, path);
		if (res != std::nullopt)
			return res;
	}
	return std::nullopt;
}

std::optional<FD::Project::Internal::FileList::RefIterator> FD::Project::Internal::FileList::find_parent(const std::string& path) {

	for (auto itr = refs.begin(), end = refs.end(); itr != end; itr++) {
		if (itr->path == path) {//top
			return std::nullopt;
		}
		auto rec = this->find_parent_recursive(itr, path);
		if (rec != std::nullopt)
			return rec;
	}
	return std::nullopt;
}

bool FD::Project::Internal::FileList::samePath(const std::string& path) {
	return this->find(path) != std::nullopt;
}

bool FD::Project::Internal::FileList::sameName(const std::string& path, const std::string& name) {
	auto itr = this->find_parent(path);

	//top
	if (itr == std::nullopt) {
		auto f = std::find_if(refs.begin(), refs.end(), [&](auto& x) {return x.name == name; });
		return f != refs.end();
	}

	auto f = std::find_if(itr.value()->dir_internal.begin(), itr.value()->dir_internal.end(), [&](auto& x) {return x.name == name; });
	return f != itr.value()->dir_internal.end();
}

std::vector<FD::Project::Internal::FileList::Ref>* FD::Project::Internal::FileList::get() noexcept {
	return &refs;
}






















