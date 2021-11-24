#ifndef CJ_MINESWEEPER_3D
#define CJ_MINESWEEPER_3D

#include "Grid3d.hpp"
#include "CubeEngine.hpp"
#include "CubeEngineDefs.hpp"

namespace cj {
	struct MinesweeperTile {
		bool flagged;
		bool revealed;
		bool mine;
		int number;
	};
	class Minesweeper3d {
	public:
		Minesweeper3d() {};
		~Minesweeper3d() {
			destroy();
		}

		
		Grid3d<MinesweeperTile> logicBoard;
		Grid3d<InstancedData> drawingBoard;
		glm::mat4 projection;
		Camera camera;
		CubeEngine engine;
		Shader shader;
		Texture texture;

		void init(int sizeX, int sizeY, int sizeZ, int bombCount) {
			logicBoard.create(sizeX, sizeY, sizeZ);
			drawingBoard.create(sizeX, sizeY, sizeZ);

			for (int x = 0; x < sizeX; x++) {
				for (int y = 0; y < sizeY; y++) {
					for (int z = 0; z < sizeZ; z++) {
						InstancedData data;
						data.model = glm::identity<glm::mat4>();
						data.model = glm::translate(data.model, glm::vec3(x, y, z));
						data.texCoords = glm::vec4(0.0f, 1.0f / 15.0f, 0.5f, 1.0f);
						drawingBoard.set(data, x, y, z);
					}
				}
			}
			engine.init();
			engine.instancedBuffer.update(drawingBoard.m_array, drawingBoard.totalSize(), sizeof(InstancedData), 0);

			//More rendering setup
			projection = glm::perspective(glm::radians(45.0f), (float)8 / (float)6, 0.1f, 100.0f);
			camera.sensitivity = 0.1f;

			shader.loadFromFiles("../shaders/vertex.glsl", "../shaders/frag.glsl");
			shader.use();

			texture.loadFromFile("../assets/fullTexture.png");
			texture.glLoad();
			Texture::activeTexture(0);
			texture.bind();
		}

		void processEvent(const sf::Event& event, const sf::Window& window) {
			if (event.type == sf::Event::Resized) {
				projection = glm::perspective(glm::radians(45.0f), (float)window.getSize().x / (float)window.getSize().y, 0.1f, 100.0f);
			}
			camera.processEvent(event);
		}

		void update(float dt) {
			camera.variableUpdate();
		}

		void draw() {
			shader.use();
			shader.setMat4("pv", projection * camera.viewMatrix);

			engine.VAO.bind();
			std::cout << drawingBoard.totalSize() << std::endl;
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, drawingBoard.totalSize());
			glBindVertexArray(0);
		}

		void destroy() {

		}

	};
}

#endif