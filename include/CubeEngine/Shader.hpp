#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

//Represents an openGL Shader program
class Shader {
public:
	Shader() : id(0) {};
	~Shader() {
		glDeleteProgram(id);
	}
	void use() {
		glUseProgram(id);
	}
	void loadFromFiles(std::string vertexShader, std::string fragmentShader) {
		unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
		id = linkShaders(vs, fs);
	}
	unsigned int id;

	//Stuff for compiling/linking
	unsigned int compileShader(int type, std::string fileName) {
		std::string tmp = readFile(fileName);
		const char* src = tmp.c_str();
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << fileName << " compile err \n" << infoLog << "\n";
		}
		return shader;
	}
	unsigned int linkShaders(unsigned int vertexShader, unsigned int fragmentShader) {
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << " link err \n" << infoLog << "\n";
		}

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return shaderProgram;
	}
private:
	std::string readFile(std::string fileName) {
		std::ifstream file(fileName);
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();
		return stream.str();
	}



	//Uniforms
public:
	unsigned int getUniformLocation(std::string name) {
		return glGetUniformLocation(id, name.c_str());
	}
	//Set int
	void setInt(const std::string& name, int x) {
		glUniform1i(getUniformLocation(name), x);
	}
	//Floats
	void setVec2(const std::string& name, float x1, float x2) {
		glUniform2f(getUniformLocation(name), x1, x2);
	}
	void setVec2(const std::string& name, const glm::fvec2& vec) {
		glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(vec));
	}

	void setVec3(const std::string& name, float x1, float x2, float x3) {
		glUniform3f(getUniformLocation(name), x1, x2, x3);
	}
	void setVec3(const std::string& name, const glm::fvec3& vec) {
		glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vec));
	}

	void setVec4(const std::string& name, float x1, float x2, float x3, float x4) {
		glUniform4f(getUniformLocation(name), x1, x2, x3, x4);
	}
	void setVec4(const std::string& name, const glm::fvec4& vec) {
		glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(vec));
	}

	void setMat2(const std::string& name, const glm::fmat2& mat) {
		glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void setMat3(const std::string& name, const glm::fmat3& mat) {
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void setMat4(const std::string& name, const glm::fmat4& mat) {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	//Doubles
	void setVec2(const std::string& name, double x1, double x2) {
		glUniform2d(getUniformLocation(name), x1, x2);
	}
	void setVec2(const std::string& name, const glm::dvec2& vec) {
		glUniform2dv(getUniformLocation(name), 1, glm::value_ptr(vec));
	}

	void setVec3(const std::string& name, double x1, double x2, double x3) {
		glUniform3d(getUniformLocation(name), x1, x2, x3);
	}
	void setVec3(const std::string& name, const glm::dvec3& vec) {
		glUniform3dv(getUniformLocation(name), 1, glm::value_ptr(vec));
	}

	void setVec4(const std::string& name, double x1, double x2, double x3, double x4) {
		glUniform4d(getUniformLocation(name), x1, x2, x3, x4);
	}
	void setVec4(const std::string& name, const glm::dvec4& vec) {
		glUniform4dv(getUniformLocation(name), 1, glm::value_ptr(vec));
	}

	void setMat2(const std::string& name, const glm::dmat2& mat) {
		glUniformMatrix2dv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void setMat3(const std::string& name, const glm::dmat3& mat) {
		glUniformMatrix3dv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void setMat4(const std::string& name, const glm::dmat4& mat) {
		glUniformMatrix4dv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}
};