#ifndef CJ_VERTEX_BUFFER_HPP
#define CJ_VERTEX_BUFFER_HPP


#include "glm/glm.hpp"

namespace cj {

	enum class VertexBufferUsage {
		Static, //Data that doesn't change
		Dynamic, //Data that changes sometimes
		Stream //Data that changes tons of times
	};
	class VertexBuffer {
	public:
		VertexBuffer() {};
		~VertexBuffer() {
			destroy();
		}
		VertexBuffer(VertexBufferUsage usage) : m_usage(usage) {};
		//VertexBuffer(const VertexBuffer&);


		void update(const VertexBuffer& that) {
			glBindBuffer(GL_COPY_READ_BUFFER, that.m_id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, m_id);

			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, that.m_size * sizeof(Vertex));

			glBindBuffer(GL_COPY_READ_BUFFER, 0);
			glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		}


		void create(void* list, size_t nItems, size_t itemSize) {
			if (!m_id)
				glGenBuffers(1, &m_id);

			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, nItems * itemSize, list, usageToGlEnum(m_usage));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_size = nItems;
		}

		void create(void* list, size_t nItems, size_t itemSize, VertexBufferUsage usage) {
			setUsage(usage);
			create(list, nItems, itemSize);
		}


		void update(void* list, size_t nItems, size_t itemSize, size_t offsetItems = 0) {
			bind();

			//Check if the buffer needs to be re-sized
			if (nItems > m_size) {
				glBufferData(GL_ARRAY_BUFFER, nItems * itemSize, list, usageToGlEnum(m_usage));

				m_size = nItems;
			}

			glBufferSubData(GL_ARRAY_BUFFER, offsetItems * itemSize, nItems * itemSize, list);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void bind() {
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		void destroy() {
			if (m_id) {
				glDeleteBuffers(1, &m_id);
				m_id = 0;
			}
		}

		size_t getItemCount() const { return m_size; };
		unsigned int getID() const { return m_id; };
		VertexBufferUsage getUsage() const { return m_usage; };

		void setUsage(VertexBufferUsage usage) {
			m_usage = usage;
		}
	private:
		size_t m_size = 0;
		unsigned int m_id = 0;
		VertexBufferUsage m_usage = VertexBufferUsage::Static;

		GLenum usageToGlEnum(VertexBufferUsage u) {
			switch (u) {
			case VertexBufferUsage::Static: return GL_STATIC_DRAW;
			case VertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
			default: return GL_STREAM_DRAW;
			}
		}
	};
}

#endif