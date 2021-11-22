#pragma once
#include <string>
#include <sys/stat.h>

#include "GL/glew.h"

#include "Texture.hpp"

#include <iostream>
#include "stb_image.h"

namespace cj {
	class Texture {
	public:
		Texture(std::string filename) {
			this->_filename = filename;

			if (!fileExists(_filename)) {
				//Do some exception stuff
				std::cout << _filename << " does not exist" << std::endl;
			}
			imageData = stbi_load(_filename.c_str(), &width, &height, &nChannels, 0);
			if (!imageData) {
				std::cout << _filename << " yielded no data" << std::endl;
			}
		}
		~Texture() {
			destroy();
		}
		void bind() {
			glBindTexture(GL_TEXTURE_2D, id);
		}
		//Loads the texture into openGL
		void glLoad() {
			glGenTextures(1, &id);
			bind();

			GLenum format = GL_RG;
			if (nChannels == 1)
				format = GL_RED;
			else if (nChannels == 3)
				format = GL_RGB;
			else if (nChannels == 4)
				format = GL_RGBA;


			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			stbi_image_free(imageData);
		}

		unsigned int getId() {
			return id;
		}

		void destroy() {
			if (id) {
				glDeleteTextures(1, &id);
				id = 0;
			}
		}

		//Activates GL_TEXTURE[spot], so try and make sure it's within GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS -1
		static void activeTexture(int spot) {
			glActiveTexture(GL_TEXTURE0 + spot);
		}

	private:
		unsigned int id;
		std::string _filename;


		int width, height, nChannels;
		unsigned char* imageData;


		static bool fileExists(const std::string& name) {
			struct stat buffer;
			return (stat(name.c_str(), &buffer) == 0);
		}
	};
}