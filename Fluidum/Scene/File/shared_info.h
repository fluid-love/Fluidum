#pragma once

#include "../Common/common.h"

namespace FS::File::Add {

	struct SharedInfo final {
		std::string path{};

		bool project = true; //project or user
		bool create = false; //true -> create, false -> cancel 

	public:
		enum class Type : UIF8 {
			File,
			Directory,
			Open
		};
		Type type{};

	};

}