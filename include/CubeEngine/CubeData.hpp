#ifndef CJ_CUBE_DATA_HPP
#define CJ_CUBE_DATA_HPP

#include <array>

namespace cj {
	//Contains data for a cube
	//Nothing more
	class CubeData {
	public:
		std::array<float, 24> cubeVertices = {
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f
		};

		std::array<unsigned int, 36> cubeIndices = {
			0, 1, 2,
			2, 1, 3,
			2, 3, 4,
			4, 3, 5,
			4, 5, 6,
			6, 5, 7,
			6, 7, 0,
			0, 7, 1,
			1, 7, 3,
			3, 7, 5,
			6, 0, 4,
			4, 0, 2
		};

		//UV coords

		std::array<float, 8> cubeUvCoords = {
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};
		std::array<unsigned int, 36> cubeUvIndices = {
			0, 1, 2,
			2, 1, 3,
			0, 1, 2,
			2, 1, 3,
			3, 2, 1,
			1, 2, 0,
			0, 1, 2,
			2, 1, 3,
			0, 1, 2,
			2, 1, 3,
			0, 1, 2,
			2, 1, 3
		};
	};
}

#endif

