#pragma once

#include "../../../Utils/include.h"

namespace FDR::Command::Internal {

	void call();

}

namespace FDR::Command::Internal {

	void setSetPos(const IF32 x, const IF32 y) noexcept;

	void setSetSize(const IF32 width, const IF32 height) noexcept;

	void setSetPosSize(const IF32 x, const IF32 y, const IF32 width, const IF32 height) noexcept;

}