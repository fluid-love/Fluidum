#pragma once

#include "include.h"

namespace FS::Utils::Class {

	constexpr inline FU::Class::ClassCode::CodeType ClassCodesView[] = {
		::FU::Class::ClassCode::GetClassCode<::FS::Analysis::Overview>(),
		::FU::Class::ClassCode::GetClassCode<::FS::TextEditor>(),
		::FU::Class::ClassCode::GetClassCode<::FS::Coding::Tab>(),
		::FU::Class::ClassCode::GetClassCode<::FS::Project::Explorer>(),
		::FU::Class::ClassCode::GetClassCode<::FS::Console>()
	};

	constexpr inline const char* ClassCodesViewNames[] = {
		"::FS::Analysis::Overview",
		"::FS::TextEditor",
		"::FS::Coding::Tab",
		"::FS::Project::Explorer",
		"::FS::Console"
	};

	static_assert(std::extent_v<decltype(ClassCodesView), 0> == std::extent_v<decltype(ClassCodesViewNames), 0>);

}