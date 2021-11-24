cmake_minimum_required(VERSION 3.10)
set(CMAKE_FIND_FRAMEWORK NEVER)

project(3dBombSearcher)

set(3D_BOMB_SEARCHER_SOURCE
	src/main.cpp
	src/stb_image_implementation.cpp
)

set(3D_BOMB_SEARCHER_HEADERS
	include/CubeEngine.hpp #Cube Engine Headers
	include/CubeEngine/VertexBuffer.hpp
	include/CubeEngine/VertexArray.hpp
	include/CubeEngine/Shader.hpp
	include/CubeEngine/CubeData.hpp
	include/CubeEngine/Camera.hpp
	include/CubeEngine/stb_image.h
	include/CubeEngine/Texture.hpp
	include/CubeEngineDefs.hpp
	include/Grid3d.hpp
	include/Minesweeper3d.hpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(BUILD_SHARED_LIBS OFF)
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_NETWORK OFF)



#_CRT_SECURE_NO_WARNINGS
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(extern/SFML-2.5.1)
add_subdirectory(extern/glm)
add_subdirectory("extern/glew-2.1.0/build/cmake")
add_subdirectory(extern/ImGui)

add_executable(${PROJECT_NAME} ${3D_BOMB_SEARCHER_SOURCE} ${3D_BOMB_SEARCHER_HEADERS})


target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PUBLIC "extern/glew-2.1.0/include")

target_link_libraries(${PROJECT_NAME} sfml-window sfml-system)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} glew_s)