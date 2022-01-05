#pragma once

#include "../../Common/common.h"

namespace FS::Project::Add {

	struct SharedInfo final {
		std::string path{};

		bool project = true;//project or user
		bool create = false;//true -> create, false -> cancel 

	public:
		enum class Type : uint8_t {
			File,
			Directory,
			Open
		};
		Type type{};
	};

}