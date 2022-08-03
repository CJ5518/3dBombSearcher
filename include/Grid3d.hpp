#ifndef CJ_3D_GRID
#define CJ_3D_GRID

//Sortof general purpose 3d grid structure
namespace cj{
	template<class T>
	class Grid3d {
	public:
		Grid3d() {};
		~Grid3d() {
			destroy();
		};

		T* m_array = nullptr;

		void create(size_t sizeX, size_t sizeY, size_t sizeZ) {
			m_sizeX = sizeX;
			m_sizeY = sizeY;
			m_sizeZ = sizeZ;
			m_array = new T[sizeX * sizeY * sizeZ];
		}

		T& get(size_t xIdx, size_t yIdx, size_t zIdx) {
			return m_array[coordsToIdx(xIdx, yIdx, zIdx)];
		}

		T& get(size_t idx) {
			return m_array[idx];
		}

		void set(const T& thing, size_t xIdx, size_t yIdx, size_t zIdx) {
			m_array[coordsToIdx(xIdx, yIdx, zIdx)] = thing;
		}

		size_t coordsToIdx(size_t xIdx, size_t yIdx, size_t zIdx) {
			return xIdx + (m_sizeX * yIdx) + (m_sizeX * m_sizeY * zIdx);
		}

		//https://softwareengineering.stackexchange.com/a/212813
		void idxToCoord(int idx, int* outX, int* outY, int* outZ) {
			*outX = idx % m_sizeX;
			*outY = (idx / m_sizeX) % m_sizeY;
			*outZ = idx / (m_sizeX * m_sizeY);
		}

		size_t sizeX() {
			return m_sizeX;
		}

		size_t sizeY() {
			return m_sizeY;
		}

		size_t sizeZ() {
			return m_sizeZ;
		}

		size_t totalSize() {
			return m_sizeZ * m_sizeY * m_sizeX;
		}

		void destroy() {
			delete[] m_array;
		}

		//Places the neighbors of idx into the out array
		//out must be of at least size 26
		//invalid indices will be negative, no numbers too large will be placed into the output array
		//the valid indices will all be at the front, and then an invalid will be placed as a kind of null terminator
		//the rest of the indices in the array will not be touched
		//All you have to check is if they are >= 0, and then they will be valid
		void getNeighborIndices(int idx, int* out) {
			int x, y, z;
			idxToCoord(idx, &x, &y, &z);
			x--;
			y--;
			z--;
			int neighborsFound = 0;
			//Go over all the neighbors
			for (int xLoop = x; xLoop <= x + 2; xLoop++) {
				//If x is invalid, skip
				if (xLoop < 0 || xLoop >= m_sizeX) {
					continue;
				}
				for (int yLoop = y; yLoop <= y + 2; yLoop++) {
					//If y is invalid, skip
					if (yLoop < 0 || yLoop >= m_sizeY) {
						continue;
					}
					for (int zLoop = z; zLoop <= z + 2; zLoop++) {
						//If z is invalid, skip
						if (zLoop < 0 || zLoop >= m_sizeZ) {
							continue;
						}
						//The case where we land on the current cell, skip
						if (xLoop == x + 1 && yLoop == y + 1 && zLoop == z + 1) {
							continue;
						}
						//We have valid coords of the neighbor cell now
						out[neighborsFound] = coordsToIdx(xLoop, yLoop, zLoop);
						neighborsFound++;
					}
				}
			}
			if (neighborsFound < 26) {
				out[neighborsFound] = -1;
			}
		}

		//Delete copy constructor and such
		Grid3d(const Grid3d&) = delete;
		Grid3d& operator=(const Grid3d&) = delete;
	private:
		size_t m_sizeX, m_sizeY, m_sizeZ;
	};
}

#endif