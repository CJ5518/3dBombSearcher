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
		//Delete copy constructor and such
		Grid3d(const Grid3d&) = delete;
		Grid3d& operator=(const Grid3d&) = delete;
	private:
		size_t m_sizeX, m_sizeY, m_sizeZ;
	};
}

#endif