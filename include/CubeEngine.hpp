#ifndef CJ_CUBE_ENGINE_HPP
#define CJ_CUBE_ENGINE_HPP

#include "CubeEngineDefs.hpp"
#include "CubeEngine/Camera.hpp"
#include "CubeEngine/CubeData.hpp"
#include "CubeEngine/Shader.hpp"
#include "CubeEngine/stb_image.h"
#include "CubeEngine/Texture.hpp"
#include "CubeEngine/VertexArray.hpp"
#include "CubeEngine/VertexBuffer.hpp"

namespace cj {
	class CubeEngine {
	public:
		//Default constructor, does nothing
		CubeEngine() {};
		//Init the cube engine
		void init() {
			CubeData cubeData;
			std::vector<Vertex> vertices;
			for (int q = 0; q < cubeData.cubeIndices.size(); q++) {
				Vertex vert;
				vert.position.x = cubeData.cubeVertices[cubeData.cubeIndices[q] * 3];
				vert.position.y = cubeData.cubeVertices[(cubeData.cubeIndices[q] * 3) + 1];
				vert.position.z = cubeData.cubeVertices[(cubeData.cubeIndices[q] * 3) + 2];
				vert.normal = glm::vec3(1.0f);
				vert.uvCoords.x = cubeData.cubeUvCoords[cubeData.cubeUvIndices[q] * 2];
				vert.uvCoords.y = cubeData.cubeUvCoords[(cubeData.cubeUvIndices[q] * 2) + 1];
				vertices.push_back(vert);
			}

			VAO.create();
			VAO.bind();

			//Create VBO
			VBO.create(vertices.data(), vertices.size(), sizeof(Vertex));
			VBO.bind();

			VAO.setBufferAttribute(sizeof(glm::vec3));
			VAO.setBufferAttribute(sizeof(glm::vec3));
			VAO.setBufferAttribute(sizeof(glm::vec2));
			VAO.flushAttributes();

			VAO.bind();
			instancedBuffer.create(0, 0, sizeof(InstancedData), VertexBufferUsage::Stream);
			instancedBuffer.bind();
			VAO.setBufferAttribute(sizeof(glm::vec4), true);
			VAO.setBufferAttribute(sizeof(glm::vec4), true);
			VAO.setBufferAttribute(sizeof(glm::vec4), true);
			VAO.setBufferAttribute(sizeof(glm::vec4), true);
			VAO.setBufferAttribute(sizeof(glm::vec4), true);
			VAO.flushAttributes();
		}

		VertexArray VAO;
		VertexBuffer VBO;
		VertexBuffer instancedBuffer;

		//Delete copy constructor and such
		CubeEngine(const CubeEngine&) = delete;
		CubeEngine& operator=(const CubeEngine&) = delete;
	};
}

#endif