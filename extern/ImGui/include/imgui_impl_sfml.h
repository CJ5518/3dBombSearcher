// dear imgui: Platform Binding for SFML
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)
// (Info: SFML is a cross-platform general purpose library for handling windows, inputs, graphics context creation, etc.)

//ONLY TESTED ON WIN32

// Missing features:
//  [ ] Platform: Mouse cursor shape and visibility. Disable with 'io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange'.
//  [ ] Platform: Clipboard support.
//  [X] Platform: Keyboard arrays indexed using SDL_SCANCODE_* codes, e.g. ImGui::IsKeyPressed(SDL_SCANCODE_SPACE).
//  [ ] Platform: Gamepad support. Enabled with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.
//  [ ] Platform: SDL2 handling of IME under Windows appears to be broken and it explicitly disable the regular Windows IME. You can restore Windows IME by compiling SDL with SDL_DISABLE_WINDOWS_IME.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#include "SFML/Window.hpp"

IMGUI_IMPL_API bool     ImGui_ImplSFML_InitForOpenGL(sf::Window* window);
IMGUI_IMPL_API bool     ImGui_ImplSFML_InitForVulkan(sf::Window* window);
IMGUI_IMPL_API bool     ImGui_ImplSFML_InitForD3D(sf::Window* window);
IMGUI_IMPL_API bool     ImGui_ImplSFML_InitForMetal(sf::Window* window);
IMGUI_IMPL_API void     ImGui_ImplSFML_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplSFML_NewFrame(sf::Window* window);
IMGUI_IMPL_API bool     ImGui_ImplSFML_ProcessEvent(sf::Event event);
