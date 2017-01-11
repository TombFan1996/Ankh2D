#ifndef SHADER_H
#define SHADER_H

#include <glm\gtx\transform.hpp>
#include <GL\glew.h>
#include <fstream>
#include <stdint.h>
#include <string>

#include "logger.h"
#include "sprite.h"

const char* loadShader(const char* _filename);
void checkShader(GLuint _shader, GLuint _flag, bool _isProgram);
GLuint createShader(const char* _buffer, GLenum _shaderType);

class Shader
{
	public:
		Shader(char* _filename, float _width, float _height);
		void update(Sprite& _sprite);
		GLuint getProgram(){return m_program;}
		~Shader();
	private:
		glm::mat4 m_perspective;
		GLuint m_program;

		enum
		{
			VERTEX,
			FRAGMENT,
			NUM_SHADERS
		};

		enum
		{
			MODEL,
			PROJECTION,
			NUM_UNIFORMS
		};

		//vert & frag
		GLuint m_shaders[NUM_SHADERS];
		GLuint m_uniforms[NUM_UNIFORMS];
};

#endif