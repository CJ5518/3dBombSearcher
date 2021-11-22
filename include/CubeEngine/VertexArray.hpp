#ifndef CJ_VERTEX_ARRAY_HPP
#define CJ_VERTEX_ARRAY_HPP

#include <vector>
#include "CubeEngine/VertexBuffer.hpp"

namespace cj {
	//Wrapper around opengls vertex array
	class VertexArray {
	public:
		VertexArray() {};
		//Delete the vertex array
		~VertexArray() {
			glDeleteVertexArrays(1, &m_id);
		}
		//Delete those pesky operators!
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		//Creates the vertex array
		void create() {
			glGenVertexArrays(1, &m_id);
		}
		//Stages an attribute on the vertex array
		void setBufferAttribute(int sizeofType, bool instanced = false) {
			m_attributeList.push_back(std::pair<short, bool>(sizeofType, instanced));
		}
		//Sets the staged attributes of the vertex array
		void flushAttributes() {
			bind();

			//Calculate the stride beforehand
			int stride = 0;
			for (int q = 0; q < m_attributeList.size(); q++) {
				stride += m_attributeList[q].first;
			}

			int offset = 0;
			for (int q = 0; q < m_attributeList.size(); q++) {
				//The size of the data type
				int x = m_attributeList[q].first;
				int index = q + m_index;
				bool instanced = m_attributeList[q].second;
				enable(index);
				//We assume these are floats
				glVertexAttribPointer(index, x / 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
				//I guess you need this for instanced rendering or smthn
				if (instanced)
					glVertexAttribDivisor(index, 1);
				offset += x;
			}
			m_index += m_attributeList.size();
			m_attributeList.clear();
		}
		//Binds the vertex array
		void bind() {
			glBindVertexArray(m_id);
		}

		unsigned int getID() {
			return m_id;
		}

		//Static member functions

		//Enable/disbale attributes on the currently bound vertex array
		static void enable(GLuint attribute) {
			glEnableVertexAttribArray((GLuint)attribute);
		}
		static void disable(GLuint attribute) {
			glDisableVertexAttribArray((GLuint)attribute);
		}
	private:
		unsigned int m_id = 0;
		std::vector<std::pair<short, bool>> m_attributeList;
		short m_index = 0;
	};
}

#endif
