#include "GL/glew.h"
#include "SFML/Window.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sfml.h"

#include "CubeEngine.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Grid3d.hpp"
#include "Minesweeper3d.hpp"

#include <iostream>

using namespace cj;

#define GL_DEBUG 1

//Used if GL_DEBUG is true
void _stdcall debugFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::cout << message << std::endl;
}

int main(int argc, char** argv) {
	//Set up opengl context settings
	sf::ContextSettings settings;
	settings.majorVersion = 4;
	settings.minorVersion = 3;
	settings.stencilBits = 8;
	settings.depthBits = 24;
	settings.antialiasingLevel = 0;
	settings.attributeFlags = sf::ContextSettings::Attribute::Core;

	//Extra debug info
#if GL_DEBUG
	settings.attributeFlags = sf::ContextSettings::Attribute::Debug;
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
	//Create the window
	sf::Window window(sf::VideoMode(800,600), "Le bon titre", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	//Setting up glew/opengl
	GLenum res = glewInit();
	if (res != GLEW_OK)
		std::cout << "glew init did not return ok: " << " " << glewGetErrorString(res);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

#if GL_DEBUG
	//This needed to be after glewInit
	glDebugMessageCallback(debugFunc, 0);
#endif
	
	Minesweeper3d minesweeper;
	minesweeper.init(3,3,3,3);

	//Set up a shader and texture
	//Needs to be a pointer otherwise weird stuff happens and the shader gets deleted or something
	//https://stackoverflow.com/questions/34258283/

	//Set up ImGui
	ImGui::CreateContext();
	ImGui_ImplSFML_InitForOpenGL(&window);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	//The main loop
	sf::Event event;
	sf::Clock deltaClock;
	bool running = true;
	while (running) {
		//Process events
		while (window.pollEvent(event)) {
			ImGui_ImplSFML_ProcessEvent(event);
			minesweeper.processEvent(event, window);
			//camera.processEvent(event);
			if (event.type == sf::Event::Closed) {
				running = false;
			}

			if (event.type == sf::Event::Resized) {
				glViewport(0, 0, window.getSize().x, window.getSize().y);
			}
		}
		//Variable update
		minesweeper.update(deltaClock.restart().asSeconds());

		//Draw things
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		minesweeper.draw();
		
		//ImGui step

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSFML_NewFrame(&window);
		ImGui::NewFrame();

		ImGui::Begin("Window");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		window.display();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSFML_Shutdown();
	ImGui::DestroyContext();
	window.close();

	return 0;
}
