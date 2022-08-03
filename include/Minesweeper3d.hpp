#ifndef CJ_MINESWEEPER_3D
#define CJ_MINESWEEPER_3D

#include "imgui.h"
//Cj headers
#include "Grid3d.hpp"
#include "CubeEngine.hpp"
#include "CubeEngineDefs.hpp"

namespace cj {
	//A tile in the minesweeper board
	struct MinesweeperTile {
		bool flagged;
		bool revealed;
		bool mine;
		int number;
	};

	//Minesweeper class, handles logic and drawing of the minesweeper
	class Minesweeper3d {
	public:
		//Default constructor
		Minesweeper3d(sf::Window& window) : window(window) {};
		~Minesweeper3d() {
			destroy();
		}

		sf::Window& window;

		//A grid of the tiles, used for logic
		Grid3d<MinesweeperTile> logicBoard;
		//Another grid, this time holding drawing data
		Grid3d<InstancedData> drawingBoard;
		//Does the drawing board need to be updated?
		//Things that mess with it should change this value themselves
		bool drawingBoardChanged = false;
		//Projection matrix
		glm::mat4 projection;
		//Camera
		Camera camera;
		//Drawing engine
		CubeEngine engine;
		//The one shader that we use
		Shader shader;
		//The one texture that we use
		Texture texture;
		//The idx of the block we are hovering over with the mouse
		int hoverBlockIdx = -1;
		//The factor applied to the distance between the blocks, 1 is edges touching, 2 is one block apart, etc.
		float blockDistanceX = 1.0f;
		float blockDistanceY = 1.0f;
		float blockDistanceZ = 1.0f;

		//ImGui board generation things
		int imGuiSizeX = 0;
		int imGuiSizeY = 0;
		int imGuiSizeZ = 0;
		int imGuiBombCount = 0;

		//Kindof a dumb function that gets the right texture coords from the minesweeper number
		//Dumb in the way it works internally
		glm::vec4 getTextureCoordsFromNumber(int n) {
			//If it's less than 0 it is unrevealed
			if (n < 0) {
				n = 29;
			}
			int x = n - 1;
			int y = 0;
			if (n >= 16 || n == 0) {
				y = 1;
				x = n - 16;
			}
			if (n == 0) {
				x = 12;
			}
			//I don't remember how or why this worked
			return glm::vec4(x / 15.0f, (x + 1) / 15.0f, 1.0f - ((y + 1) / 2.0f), 1.0f * ((2-y) / 2.0f));
		}

		//Initialize the minesweeper
		void init(int sizeX, int sizeY, int sizeZ, int bombCount) {
			//Init the drawing
			engine.init();
			initBoard(sizeX, sizeY, sizeZ, bombCount);
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

		void initBoard(int sizeX, int sizeY, int sizeZ, int bombCount) {
			//Create the logic and drawing boards
			logicBoard.create(sizeX, sizeY, sizeZ);
			drawingBoard.create(sizeX, sizeY, sizeZ);

			//Cap the bomb count
			if (bombCount >= sizeX * sizeY * sizeZ) {
				bombCount = (sizeX * sizeY * sizeZ) - 2;
			}

			imGuiSizeX = sizeX;
			imGuiSizeY = sizeY;
			imGuiSizeZ = sizeZ;
			imGuiBombCount = bombCount;
			drawingBoardChanged = true;

			//Set all the tiles to 0
			memset(logicBoard.m_array, 0, logicBoard.totalSize() * sizeof(MinesweeperTile));

			//Place bombs randomly
			for (int q = 0; q < bombCount; q++) {
				while (true) {
					int tileChoice = rand() % logicBoard.totalSize();
					if (!logicBoard.get(tileChoice).mine) {
						logicBoard.get(tileChoice).mine = true;
						break;
					}
				}
			}

			//For every tile
			for (int x = 0; x < sizeX; x++) {
				for (int y = 0; y < sizeY; y++) {
					for (int z = 0; z < sizeZ; z++) {
						//Calculate the number on this block
						MinesweeperTile tile = logicBoard.get(x, y, z);
						int neighborIndices[26];
						logicBoard.getNeighborIndices(logicBoard.coordsToIdx(x, y, z), neighborIndices);
						for (int q = 0; q < 26; q++) {
							if (neighborIndices[q] < 0) break;
							if (logicBoard.get(neighborIndices[q]).mine) {
								tile.number++;
							}
						}
						tile.revealed = true;

						//Gen the drawing data
						InstancedData data;
						data.model = glm::identity<glm::mat4>();
						//data.model = glm::scale(data.model, glm::vec3(2, 2, 2));
						data.model = glm::translate(glm::identity<glm::mat4>(),
							glm::vec3(x * blockDistanceX, y * blockDistanceY, z * blockDistanceZ)
						);
						data.texCoords = getTextureCoordsFromNumber(tile.mine ? 29 : tile.number);//glm::vec4(0.0f, 1.0f / 15.0f, 0.5f, 1.0f);
						drawingBoard.set(data, x, y, z);
						//This is right btw
						//printf("%d %d %d, %f %f %f, scale: %f\n", x, y, z, data.model[3][0], data.model[3][1], data.model[3][2], data.model[0][0]);
					}
				}
			}
		}

		//Handle user input
		void processEvent(const sf::Event& event, const sf::Window& window) {
			if (event.type == sf::Event::Resized) {
				projection = glm::perspective(glm::radians(45.0f), (float)window.getSize().x / (float)window.getSize().y, 0.1f, 100.0f);
			}
			camera.processEvent(event);
		}

		void update(float dt) {
			camera.variableUpdate();

			//Set the block we're hovering over to be highlighted
			//https://stackoverflow.com/a/56348846
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
			sf::Vector2f halfScreen = (sf::Vector2f)window.getSize() / 2.0f;
			glm::mat4 invMat = glm::inverse(projection * camera.viewMatrix);
			glm::vec4 near = glm::vec4((mousePos.x - halfScreen.x) / halfScreen.x, -1 * (mousePos.y - halfScreen.y) / halfScreen.y, -1, 1.0);
			glm::vec4 far = glm::vec4((mousePos.x - halfScreen.x) / halfScreen.x, -1 * (mousePos.y - halfScreen.y) / halfScreen.y, 1, 1.0);
			glm::vec4 nearResult = invMat * near;
			glm::vec4 farResult = invMat * far;
			nearResult /= nearResult.w;
			farResult /= farResult.w;
			glm::vec3 dir = glm::normalize(glm::vec3(farResult - nearResult));
			hoverBlockIdx = raycast(camera.position, dir);
		}
		
		//Draw the minesweeper
		void draw() {
			//Update the instanced buffer to reflect the new data
			if (drawingBoardChanged) {
				engine.instancedBuffer.update(drawingBoard.m_array, drawingBoard.totalSize(), sizeof(InstancedData), 0);
				drawingBoardChanged = false;
			}

			shader.use();
			shader.setMat4("pv", projection * camera.viewMatrix);

			shader.setInt("selectedID", hoverBlockIdx);

			engine.VAO.bind();
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, drawingBoard.totalSize());
			glBindVertexArray(0);
		}

		float del = 0.0f;
		void imguiStep() {
			ImGui::Begin("Bombsearcher");

			//Distance between the blocks
			ImGui::Text("Block distance");
			float old = blockDistanceX;
			ImGui::SliderFloat("X##sliderX", &blockDistanceX, 1.0f, 5.0f);
			drawingBoardChanged |= old != blockDistanceX;
			old = blockDistanceY;
			ImGui::SliderFloat("Y##sliderY", &blockDistanceY, 1.0f, 5.0f);
			drawingBoardChanged |= old != blockDistanceY;
			old = blockDistanceZ;
			ImGui::SliderFloat("Z##sliderZ", &blockDistanceZ, 1.0f, 5.0f);
			drawingBoardChanged |= old != blockDistanceZ;

			//If things changed, update the drawing board
			if (drawingBoardChanged) {
				for (int q = 0; q < drawingBoard.totalSize(); q++) {
					int x, y, z;
					drawingBoard.idxToCoord(q, &x, &y, &z);
					//data.model = glm::scale(data.model, glm::vec3(2, 2, 2));
					drawingBoard.get(q).model = glm::translate(glm::identity<glm::mat4>(),
						glm::vec3(x * blockDistanceX, y * blockDistanceY, z * blockDistanceZ)
					);
				}
			}

			ImGui::Separator();
			ImGui::Text("Game size");
			ImGui::InputInt("X", &imGuiSizeX, 1, 5);
			ImGui::InputInt("Y", &imGuiSizeY, 1, 5);
			ImGui::InputInt("Z", &imGuiSizeZ, 1, 5);


			
			ImGui::InputInt("Bombs", &imGuiBombCount, 1, 20);
			if (ImGui::Button("Generate field")) {
				initBoard(imGuiSizeX, imGuiSizeY, imGuiSizeZ, imGuiBombCount);
			}

			ImGui::End();
		}

		void destroy() {
			engine.destroy();
		}

		//Do a raycast in this little minesweeper world
		//Returns the index of the hit minesweeper block
		//Please make direction a unit vector

		//Max distance away from the camera to march
		const float rayCastMaxDistance = 30.0f;
		//March step size, lower values are more accurate but slower
		const float rayCastStepDistance = 0.5f;
		int raycast(glm::vec3 position, glm::vec3 direction) {
			glm::vec3 checkingPoint = position + (direction * rayCastStepDistance);

			while (glm::length(position - checkingPoint) <= rayCastMaxDistance) {
				//Check all the boxes
				for (int q = 0; q < drawingBoard.totalSize(); q++) {
					InstancedData cubeData = drawingBoard.get(q);
					//The center of the cube
					glm::vec3 cubeCenter = glm::vec3(cubeData.model[3][0], cubeData.model[3][1], cubeData.model[3][2]);
					float cubeSize = cubeData.model[0][0];
					//Check if the checking point is inside of the cube
					//Cubes are axis aligned always which is pretty cool
					//if ()
					if (glm::length(cubeCenter - checkingPoint) <= 1.0f) {
						return q;
					}

				}
				checkingPoint = checkingPoint + (direction * rayCastStepDistance);
			}

			return -1;
		}

	};
}

#endif