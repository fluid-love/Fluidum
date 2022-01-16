#pragma once

#include "include.h"

namespace FS::Utils::Class {

	constexpr inline FU::Class::ClassCode::CodeType ClassCodesView[] = {
		::FU::Class::ClassCode::GetClassCode<::FS::Analysis::Overview>(),
		::FU::Class::ClassCode::GetClassCode<::FS::TextEditor>(),
		::FU::Class::ClassCode::GetClassCode<::FS::Coding::Tab>(),
		::FU::Class::ClassCode::GetClassCode<::FS::Project::Explorer>(),
	};

	constexpr inline const char* ClassCodesViewNames[] = {
		"::FS::Analysis::Overview",
		"::FS::TextEditor",
		"::FS::Coding::Tab",
		"::FS::Project::Explorer",
	};

}