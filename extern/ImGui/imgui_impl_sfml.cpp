// dear imgui: Platform Binding for SDL2
// This needs to be used along with a Renderer (e.g. DirectX11, OpenGL3, Vulkan..)
// (Info: SDL2 is a cross-platform general purpose library for handling windows, inputs, graphics context creation, etc.)
// (Requires: SDL 2.0. Prefer SDL 2.0.4+ for full feature support.)

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

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2020-05-25: Misc: Report a zero display-size when window is minimized, to be consistent with other backends.
//  2020-02-20: Inputs: Fixed mapping for ImGuiKey_KeyPadEnter (using SDL_SCANCODE_KP_ENTER instead of SDL_SCANCODE_RETURN2).
//  2019-12-17: Inputs: On Wayland, use SDL_GetMouseState (because there is no global mouse state).
//  2019-12-05: Inputs: Added support for ImGuiMouseCursor_NotAllowed mouse cursor.
//  2019-07-21: Inputs: Added mapping for ImGuiKey_KeyPadEnter.
//  2019-04-23: Inputs: Added support for SDL_GameController (if ImGuiConfigFlags_NavEnableGamepad is set by user application).
//  2019-03-12: Misc: Preserve DisplayFramebufferScale when main window is minimized.
//  2018-12-21: Inputs: Workaround for Android/iOS which don't seem to handle focus related calls.
//  2018-11-30: Misc: Setting up io.BackendPlatformName so it can be displayed in the About Window.
//  2018-11-14: Changed the signature of ImGui_ImplSDL2_ProcessEvent() to take a 'const SDL_Event*'.
//  2018-08-01: Inputs: Workaround for Emscripten which doesn't seem to handle focus related calls.
//  2018-06-29: Inputs: Added support for the ImGuiMouseCursor_Hand cursor.
//  2018-06-08: Misc: Extracted imgui_impl_sdl.cpp/.h away from the old combined SDL2+OpenGL/Vulkan examples.
//  2018-06-08: Misc: ImGui_ImplSDL2_InitForOpenGL() now takes a SDL_GLContext parameter.
//  2018-05-09: Misc: Fixed clipboard paste memory leak (we didn't call SDL_FreeMemory on the data returned by SDL_GetClipboardText).
//  2018-03-20: Misc: Setup io.BackendFlags ImGuiBackendFlags_HasMouseCursors flag + honor ImGuiConfigFlags_NoMouseCursorChange flag.
//  2018-02-16: Inputs: Added support for mouse cursors, honoring ImGui::GetMouseCursor() value.
//  2018-02-06: Misc: Removed call to ImGui::Shutdown() which is not available from 1.60 WIP, user needs to call CreateContext/DestroyContext themselves.
//  2018-02-06: Inputs: Added mapping for ImGuiKey_Space.
//  2018-02-05: Misc: Using SDL_GetPerformanceCounter() instead of SDL_GetTicks() to be able to handle very high framerate (1000+ FPS).
//  2018-02-05: Inputs: Keyboard mapping is using scancodes everywhere instead of a confusing mixture of keycodes and scancodes.
//  2018-01-20: Inputs: Added Horizontal Mouse Wheel support.
//  2018-01-19: Inputs: When available (SDL 2.0.4+) using SDL_CaptureMouse() to retrieve coordinates outside of client area when dragging. Otherwise (SDL 2.0.3 and before) testing for SDL_WINDOW_INPUT_FOCUS instead of SDL_WINDOW_MOUSE_FOCUS.
//  2018-01-18: Inputs: Added mapping for ImGuiKey_Insert.
//  2017-08-25: Inputs: MousePos set to -FLT_MAX,-FLT_MAX when mouse is unavailable/missing (instead of -1,-1).
//  2016-10-15: Misc: Added a void* user_data parameter to Clipboard function handlers.

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sfml.h"


#include "SFML/Window.hpp"

#define SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE    SDL_VERSION_ATLEAST(2,0,4)
#define SDL_HAS_VULKAN                      SDL_VERSION_ATLEAST(2,0,6)

//Because I didn't want to do the gamepad function at the time
#define DONT_COMPILE_TODO_THINGS

// Data
static sf::Window*  g_Window;
static sf::Clock*   g_Clock;
static bool         g_MousePressed[3] = { false, false, false };
static sf::Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = {};
static bool         g_MouseCanUseGlobalState = true;
static int          g_WindowSizeX = 0;
static int          g_WindowSizeY = 0;

static const char* ImGui_ImplSFML_GetClipboardText(void*) {
	std::string x = sf::Clipboard::getString();
	const char* y = x.c_str();
	return y;
}

static void ImGui_ImplSFML_SetClipboardText(void*, const char* text)
{
	sf::Clipboard::setString(text);
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
// If you have multiple SDL events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
bool ImGui_ImplSFML_ProcessEvent(sf::Event event) {
	ImGuiIO& io = ImGui::GetIO();
	switch (event.type)
	{
	case sf::Event::MouseWheelScrolled:
		{
			//if (event.mouseWheel.x > 0) io.MouseWheelH += 1;
			//if (event.mouseWheel.x < 0) io.MouseWheelH -= 1;
		//Dunno if it's just me but mouseWheel.delta is always 0
			if (event.mouseWheel.x > 0) io.MouseWheel += 1;
			if (event.mouseWheel.x < 0) io.MouseWheel -= 1;
			//Should put io.MouseWheel += event.mouseWheel.x neh?
			return true;
		}
	case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left) g_MousePressed[0] = true;
			if (event.mouseButton.button == sf::Mouse::Right) g_MousePressed[1] = true;
			if (event.mouseButton.button == sf::Mouse::Middle) g_MousePressed[2] = true;
			return true;
		}
	case sf::Event::TextEntered:
		{
			char temp[2] = { '1', '\0' };
			temp[0] = (const char)event.text.unicode;
			
			io.AddInputCharactersUTF8(temp);
			return true;
		}
	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
		{
			int key = event.key.code;
			IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[key] = (event.type == sf::Event::KeyPressed);
			io.KeyShift = event.key.shift;
			io.KeyCtrl = event.key.control;
			io.KeyAlt = event.key.alt;
#ifdef _WIN32
			io.KeySuper = false;
#else
			//io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
#endif
			return true;
		}
	case sf::Event::Resized:
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)event.size.width, (float)event.size.height);
	}
	return false;
}

static bool ImGui_ImplSFML_Init(sf::Window* window)
{
	g_Window = window;
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)window->getSize().x, (float)window->getSize().y);

	// Setup back-end capabilities flags
	//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
	io.BackendPlatformName = "imgui_impl_sfml";

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
	io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
	io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
	io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
	io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
	io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
	io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
	io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
	io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
	io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
	io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::Backspace;
	io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
	io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
	io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
	io.KeyMap[ImGuiKey_KeyPadEnter] = sf::Keyboard::Enter;
	io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
	io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
	io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
	io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
	io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
	io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

	io.SetClipboardTextFn = ImGui_ImplSFML_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplSFML_GetClipboardText;
	io.ClipboardUserData = NULL;

	// Load mouse cursors
	auto loadCursor = [](sf::Cursor::Type type) {
		auto ret = new sf::Cursor();
		ret->loadFromSystem(type);
		return ret;
	};
	g_MouseCursors[ImGuiMouseCursor_Arrow] = loadCursor(sf::Cursor::Arrow);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = loadCursor(sf::Cursor::Text);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = loadCursor(sf::Cursor::SizeAll);
	g_MouseCursors[ImGuiMouseCursor_ResizeNS] = loadCursor(sf::Cursor::SizeVertical);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW] = loadCursor(sf::Cursor::SizeHorizontal);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = loadCursor(sf::Cursor::SizeBottomLeftTopRight);
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = loadCursor(sf::Cursor::SizeTopLeftBottomRight);
	g_MouseCursors[ImGuiMouseCursor_Hand] = loadCursor(sf::Cursor::Hand);
	g_MouseCursors[ImGuiMouseCursor_NotAllowed] = loadCursor(sf::Cursor::NotAllowed);

	g_Clock = new sf::Clock();

	
	// Check and store if we are on Wayland
	//g_MouseCanUseGlobalState = strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;

#ifdef _WIN32
	io.ImeWindowHandle = window->getSystemHandle();
#else
	(void)window;
#endif

	return true;
}

bool ImGui_ImplSFML_InitForOpenGL(sf::Window *window)
{
	return ImGui_ImplSFML_Init(window);
}

bool ImGui_ImplSFML_InitForVulkan(sf::Window* window)
{
	return ImGui_ImplSFML_Init(window);
}

bool ImGui_ImplSFML_InitForD3D(sf::Window* window)
{
	return ImGui_ImplSFML_Init(window);
}

bool ImGui_ImplSFML_InitForMetal(sf::Window* window)
{
	return ImGui_ImplSFML_Init(window);
}

void ImGui_ImplSFML_Shutdown()
{
	g_Window = NULL;


	// Destroy sfml mouse cursors
	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
		delete g_MouseCursors[cursor_n];
	memset(g_MouseCursors, 0, sizeof(g_MouseCursors));

	delete g_Clock;
}

static void ImGui_ImplSFML_UpdateMousePosAndButtons()
{
	ImGuiIO& io = ImGui::GetIO();

	// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
	if (io.WantSetMousePos)
		sf::Mouse::setPosition(sf::Vector2i((int)io.MousePos.x, (int)io.MousePos.y), *g_Window);
	else
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

	io.MouseDown[0] = g_MousePressed[0] || sf::Mouse::isButtonPressed(sf::Mouse::Left);  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
	io.MouseDown[1] = g_MousePressed[1] || sf::Mouse::isButtonPressed(sf::Mouse::Right);
	io.MouseDown[2] = g_MousePressed[2] || sf::Mouse::isButtonPressed(sf::Mouse::Middle);

	g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;
//was SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
#if false && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
	SDL_Window* focused_window = SDL_GetKeyboardFocus();
	if (g_Window == focused_window)
	{
		if (g_MouseCanUseGlobalState)
		{
			// SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
			// The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
			// Won't use this workaround when on Wayland, as there is no global mouse position.
			int wx, wy;
			SDL_GetWindowPosition(focused_window, &wx, &wy);
			SDL_GetGlobalMouseState(&mx, &my);
			mx -= wx;
			my -= wy;
		}
		io.MousePos = ImVec2((float)mx, (float)my);
	}

	// SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
	// The function is only supported from SDL 2.0.4 (released Jan 2016)
	bool any_mouse_button_down = ImGui::IsAnyMouseDown();
	SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
#else
	if (g_Window->hasFocus()) {
		io.MousePos = ImVec2(sf::Mouse::getPosition(*g_Window).x, sf::Mouse::getPosition(*g_Window).y);
	}
#endif
}

static void ImGui_ImplSFML_UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		g_Window->setMouseCursorVisible(false);
	}
	else
	{
		// Show OS mouse cursor
		g_Window->setMouseCursor(*(g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]));
		g_Window->setMouseCursorVisible(true);
	}
}
#ifndef DONT_COMPILE_TODO_THINGS
static void ImGui_ImplSDL2_UpdateGamepads()
{
	ImGuiIO& io = ImGui::GetIO();
	memset(io.NavInputs, 0, sizeof(io.NavInputs));
	if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
		return;

	// Get gamepad
	SDL_GameController* game_controller = SDL_GameControllerOpen(0);
	if (!game_controller)
	{
		io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
		return;
	}

	// Update gamepad inputs
	#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0) ? 1.0f : 0.0f; }
	#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
	const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
	MAP_BUTTON(ImGuiNavInput_Activate,      SDL_CONTROLLER_BUTTON_A);               // Cross / A
	MAP_BUTTON(ImGuiNavInput_Cancel,        SDL_CONTROLLER_BUTTON_B);               // Circle / B
	MAP_BUTTON(ImGuiNavInput_Menu,          SDL_CONTROLLER_BUTTON_X);               // Square / X
	MAP_BUTTON(ImGuiNavInput_Input,         SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
	MAP_BUTTON(ImGuiNavInput_DpadLeft,      SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
	MAP_BUTTON(ImGuiNavInput_DpadRight,     SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
	MAP_BUTTON(ImGuiNavInput_DpadUp,        SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
	MAP_BUTTON(ImGuiNavInput_DpadDown,      SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
	MAP_BUTTON(ImGuiNavInput_FocusPrev,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
	MAP_BUTTON(ImGuiNavInput_FocusNext,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
	MAP_BUTTON(ImGuiNavInput_TweakSlow,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
	MAP_BUTTON(ImGuiNavInput_TweakFast,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
	MAP_ANALOG(ImGuiNavInput_LStickLeft,    SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
	MAP_ANALOG(ImGuiNavInput_LStickRight,   SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
	MAP_ANALOG(ImGuiNavInput_LStickUp,      SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
	MAP_ANALOG(ImGuiNavInput_LStickDown,    SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);

	io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
	#undef MAP_BUTTON
	#undef MAP_ANALOG
}
#endif // DONT_COMPILE_TODO_THINGS

void ImGui_ImplSFML_NewFrame(sf::Window* window)
{
	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	//io.DisplaySize = ImVec2((float)w, (float)h);
	//(float)display_w / w, (float)display_h / h, idk check impl sdl
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	io.DeltaTime = g_Clock->restart().asSeconds();

	ImGui_ImplSFML_UpdateMousePosAndButtons();
	ImGui_ImplSFML_UpdateMouseCursor();

	// Update game controllers (if enabled and available)
#ifndef DONT_COMPILE_TODO_THINGS
	ImGui_ImplSDL2_UpdateGamepads();
#endif // !DONT_COMPILE_TODO_THINGS
}
