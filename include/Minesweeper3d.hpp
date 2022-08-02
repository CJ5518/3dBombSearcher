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

		//Kindof a dumb function that puts the minesweeper number into the right texture coords
		glm::vec4 getTextureCoordsFromNumber(int n) {
			int x = n - 1;
			int y = 0;
			if (n >= 16 || n == 0) {
				y = 1;
				x = n - 16;
			}
			if (n == 0) {
				x = 13;
			}
			return glm::vec4(x / 15.0f, (x + 1) / 15.0f, 1.0f - ((y + 1) / 2.0f), 1.0f * ((2-y) / 2.0f));

		}

		void init(int sizeX, int sizeY, int sizeZ, int bombCount) {
			logicBoard.create(sizeX, sizeY, sizeZ);
			drawingBoard.create(sizeX, sizeY, sizeZ);

			for (int x = 0; x < sizeX; x++) {
				for (int y = 0; y < sizeY; y++) {
					for (int z = 0; z < sizeZ; z++) {
						//Gen the logical bit


						//Gen the drawing data
						InstancedData data;
						data.model = glm::identity<glm::mat4>();
						data.model = glm::translate(data.model, glm::vec3(x * 1.1f, y * 1.1f, z * 1.1f));
						data.texCoords = getTextureCoordsFromNumber(1);//glm::vec4(0.0f, 1.0f / 15.0f, 0.5f, 1.0f);
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
		int del = 0;
		void draw() {

			for (int q = 0; q < drawingBoard.totalSize(); q++) {
				drawingBoard.get(q).texCoords = getTextureCoordsFromNumber((del++ / 1200) % 28);
			}
			std::cout << (del++ / 1200) % 28 << std::endl;
			engine.instancedBuffer.update(drawingBoard.m_array, drawingBoard.totalSize(), sizeof(InstancedData), 0);

			shader.use();
			shader.setMat4("pv", projection * camera.viewMatrix);
			shader.setInt("selectedID", -1);

			engine.VAO.bind();
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, drawingBoard.totalSize());
			glBindVertexArray(0);
		}

		void destroy() {

		}

		//Do a raycast in this little minesweeper world
		//Returns the index of the hit minesweeper block
		//Please make direction a unit vector
		const float rayCastMaxDistance = 10.0f;
		const float rayCastStepDistance = 0.5f;
		int raycast(glm::vec3 position, glm::vec3 direction) {
			glm::vec3 checkingPoint;
		}

	};
}

#endif