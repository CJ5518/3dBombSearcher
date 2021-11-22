#ifndef CJ_CUBE_ENGINE_DEFS_HPP
#define CJ_CUBE_ENGINE_DEFS_HPP

#include "glm/glm.hpp"

namespace cj {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uvCoords;
	};

	struct InstancedData {
		glm::vec4 texCoords;
		glm::mat4 model;
	};
}

#endif