#pragma once

#include <FluidumVK/FVKapi.h>

#include <FluidumClass/FluidumClass.h>

#include "../../common/FDcommon.h"

#include <filesystem>
#include <mutex>
#include <variant>

namespace FD {
	namespace Object {

		template<template <typename T> typename U>
		using Keys = std::vector<U<std::string>>;

	}

	namespace Object {
		struct Vertex {
			glm::vec3 pos;
			glm::vec4 color;
		};

		using IndexSize = uint32_t;
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<IndexSize>;

	}
}

namespace FD::Object::Camera {
	struct Camera {
		glm::vec3 pos = { 0.0f,0.0f,1.0f };
		glm::vec3 center = { 0.0f,0.0f,0.0f };
		glm::vec3 up = { 0.0f,1.0f,0.0f };
		float aspect = 16.0f / 9.0f;
	};
}