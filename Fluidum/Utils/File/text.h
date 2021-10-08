#pragma once

#include "path.h"
#include "../Concept/concept.h"

namespace FU::File {

	template<const char* const Path, Concept::IsScopedEnum Enum>
	class Text final {
	private:
		using UT = std::underlying_type_t<Enum>;

	public:
		Text(const Enum index) {
			std::ifstream ifs(Path, std::ios::in);

			if (!ifs)
				throw std::runtime_error("Failed to open file.");

			std::string data;

			for (UT i = 0; i <= static_cast<UT>(index); i++) {
				std::getline(ifs, data);
			}

			this->text = data;
		}
		~Text() = default;

	private:
		std::string text = "";

	public:
		operator const std::string& ()const noexcept {
			return this->text;
		}

		operator std::string& ()const noexcept {
			return this->text;
		}
	};


}