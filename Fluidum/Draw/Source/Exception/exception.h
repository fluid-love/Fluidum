#pragma once

#include <exception>

namespace FDR::Exception {

	class Error final : public std::nested_exception {};

}