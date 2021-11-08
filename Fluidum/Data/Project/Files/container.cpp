#include "container.h"

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::add(const std::string& parent, const std::string& path, const Directory& info) {
	directories.emplace_back(std::make_shared<Directory>(info));
	Ref ref = this->makeRef(path, info);

	auto f = this->find(path);

	//already exist
	if (f != std::nullopt) {
		bool res = this->tryReplaceExistElm(f.value(), ref);
		if (res)
			return f.value().operator->();
		else
			throw std::runtime_error("Failed to add directory. \"path\" already exists");
	}

	return this->addRef(parent, ref);
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::add(const std::string& parent, const std::string& path, const Code& info) {
	codes.emplace_back(std::make_shared<Code>(info));
	Ref ref = this->makeRef(path, info);

	auto f = this->find(path);
	//already exist
	if (f != std::nullopt) {
		bool res = this->tryReplaceExistElm(f.value(), ref);
		if (res)
			return f.value().operator->();
		else
			throw std::runtime_error("Failed to add directory. \"path\" already exists");
	}

	return this->addRef(parent, ref);
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::add(const std::string& parent, const std::string& path, const Unsupported& info) {
	unsupported.emplace_back(std::make_shared<Unsupported>(info));
	Ref ref = this->makeRef(path, info);

	auto f = this->find(path);
	//already exist
	//already exist
	if (f != std::nullopt) {
		bool res = this->tryReplaceExistElm(f.value(), ref);
		if (res)
			return f.value().operator->();
		else
			throw std::runtime_error("Failed to add directory. \"path\" already exists");
	}

	return this->addRef(parent, ref);
}


FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::tryAdd(const std::string& parent, const std::string& path, const Directory& info) {
	auto f = this->find(path);

	directories.emplace_back(std::make_shared<Directory>(info));
	Ref ref = this->makeRef(path, info);

	//already exist
	if (f != std::nullopt) {
		bool res = this->tryReplaceExistElm(f.value(), ref);
		if (res)
			return f.value().operator->();
		else
			nullptr;
	}

	return this->addRef(parent, ref);
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::tryAdd(const std::string& parent, const std::string& path, const Code& info) {
	auto f = this->find(path);

	codes.emplace_back(std::make_shared<Code>(info));
	Ref ref = this->makeRef(path, info);

	//already exist
	if (f != std::nullopt) {
		bool res = this->tryReplaceExistElm(f.value(), ref);
		if (res)
			return f.value().operator->();
		else
			nullptr;
	}

	return this->addRef(parent, ref);
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::tryAdd(const std::string& parent, const std::string& path, const Unsupported& info) {
	auto f = this->find(path);

	unsupported.emplace_back(std::make_shared<Unsupported>(info));
	Ref ref = this->makeRef(path, info);

	//already exist
	if (f != std::nullopt) {
		bool res = this->tryReplaceExistElm(f.value(), ref);
		if (res)
			return f.value().operator->();
		else
			nullptr;
	}

	return this->addRef(parent, ref);
}

void FD::Project::Internal::FileList::erase(const std::string& path) {
	auto itr = this->find(path);

	auto parentItr = this->find_parent(path);
	//parentItr == std::nullopt -> top

	assert(itr != std::nullopt);

	this->deletePtr(itr.value());

	if (parentItr == std::nullopt) {
		auto f = std::find_if(refs.begin(), refs.end(), [&](auto& x) {return x.path == path; });
		refs.erase(f);
	}
	else
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

void FD::Project::Internal::FileList::sync() {
	for (auto& x : refs) {
		if (!std::filesystem::exists(x.path))
			x.exist = false;
		else
			x.exist = true;

		if (!x.dir_internal.empty())
			this->sync(x.dir_internal);
	}
}

void FD::Project::Internal::FileList::sync(std::vector<Ref>& info) {
	for (auto& x : info) {
		if (!std::filesystem::exists(x.path))
			x.exist = false;
		else
			x.exist = true;

		if (!x.dir_internal.empty())
			this->sync(x.dir_internal);
	}
}

void FD::Project::Internal::FileList::forEach_recursive(void(*function)(Ref&), Ref& ref) {
	for (auto& x : ref.dir_internal) {
		function(x);
		if (!x.dir_internal.empty())
			this->forEach_recursive(function, x);
	}
}

void FD::Project::Internal::FileList::forEach(void(*function)(Ref&)) {
	for (auto& x : refs) {
		function(x);
		this->forEach_recursive(function, x);
	}
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

void FD::Project::Internal::FileList::deletePtr(RefIterator itr) {
	if (itr->type == Type::Directory) {
		auto f = std::find_if(directories.begin(), directories.end(), [&](auto& x) {return x.get() == itr->ptr; });
		assert(f != directories.end());
		directories.erase(f);
	}
	else if (itr->type == Type::Code) {
		auto f = std::find_if(codes.begin(), codes.end(), [&](auto& x) {return x.get() == itr->ptr; });
		assert(f != codes.end());
		codes.erase(f);
	}
	else if (itr->type == Type::Unsupported) {
		auto f = std::find_if(unsupported.begin(), unsupported.end(), [&](auto& x) {return x.get() == itr->ptr; });
		assert(f != unsupported.end());
		unsupported.erase(f);
	}
}

bool FD::Project::Internal::FileList::tryReplaceExistElm(RefIterator itr, Ref& ref) {
	if (!itr->exist) {
		this->deletePtr(itr);
		*itr = ref;
		return true;
	}

	return false;
}

FD::Project::Internal::FileList::Ref* FD::Project::Internal::FileList::addRef(const std::string& parent, const Ref& ref) {
	if (parent.empty()) {
		refs.emplace_back(ref);
		return &refs.back();
	}

	//find parent
	auto itr = this->find(parent);
	assert(itr != std::nullopt);
	return &itr.value()->dir_internal.emplace_back(ref);
}

FD::Project::Internal::FileList::Ref FD::Project::Internal::FileList::makeRef(const std::string& path, const Directory& info) const {
	return {
		.type = Type::Directory,
		.path = path,
		.name = FU::File::directoryName(path),
		.ptr = directories.back().get(),
	};
}

FD::Project::Internal::FileList::Ref FD::Project::Internal::FileList::makeRef(const std::string& path, const Code& info) const {
	return {
		.type = Type::Code,
		.path = path,
		.name = FU::File::fileName(path),
		.ptr = codes.back().get(),
	};
}

FD::Project::Internal::FileList::Ref FD::Project::Internal::FileList::makeRef(const std::string& path, const Unsupported& info) const {
	return {
	.type = Type::Unsupported,
	.path = path,
	.name = FU::File::fileName(path),
	.ptr = unsupported.back().get(),
	};
}














