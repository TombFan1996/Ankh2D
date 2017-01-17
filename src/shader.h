#ifndef SHADER_H
#define SHADER_H

#include <glm\mat4x4.hpp>
#include <GL\glew.h>
#include <fstream>
#include <stdint.h>
#include <string>

#include "logger.h"

const char* loadShader(const char* _filename);
void checkShader(GLuint _shader, GLuint _flag, bool _isProgram);
GLuint createShader(const char* _buffer, GLenum _shaderType);

class Shader
{
	public:
		Shader(char* _filename);
		void getUniformMat4(GLuint _uniform, glm::mat4 _matrix4);
		GLuint& getProgram(){return m_program;}
		GLuint getUniformLocation(char* _uniform);
		~Shader();
	private:
		GLuint m_program;

		enum
		{
			VERTEX,
			FRAGMENT,
			NUM_SHADERS
		};

		//vert & frag
		GLuint m_shaders[NUM_SHADERS];
};

#endif