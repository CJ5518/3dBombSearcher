
//Open gl
#include "GL/glew.h"
//Windowing
#include "SFML/Window.hpp"
//ImGui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sfml.h"
//glm math
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Cj headers
#include "CubeEngine.hpp"
#include "Grid3d.hpp"
#include "Minesweeper3d.hpp"

//Standard libs
#include <iostream>

using namespace cj;

/*
Room for optimization:

In the minesweeper class we do a full graphics update at the drop of a hat, but we really don't need to a lot of the time
*/

#define GL_DEBUG 0

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

	srand(time(0));

	//Extra debug info
#if GL_DEBUG
	settings.attributeFlags = sf::ContextSettings::Attribute::Debug;
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
	//Create the window
	sf::Window window(sf::VideoMode(1200,900), "Le bon titre", sf::Style::Default, settings);
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
	
	Minesweeper3d minesweeper(window);
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
			//Send events to other things
			ImGui_ImplSFML_ProcessEvent(event);
			minesweeper.processEvent(event, window);
			if (event.type == sf::Event::Closed) {
				running = false;
			}

			if (event.type == sf::Event::Resized) {
				glViewport(0, 0, window.getSize().x, window.getSize().y);
			}
		}

		//Variable update
		minesweeper.update(deltaClock.restart().asSeconds());

		//---------------------------------------------

		//Draw things
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		minesweeper.draw();

		//---------------------------------------------
		
		//ImGui step

		//Init
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSFML_NewFrame(&window);
		ImGui::NewFrame();

		//Actual fun time ImGui stuff
		minesweeper.imguiStep();

		//Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		window.display();
	}
	//Shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSFML_Shutdown();
	ImGui::DestroyContext();

	window.close();

	return 0;
}
