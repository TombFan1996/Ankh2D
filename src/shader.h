#ifndef SHADER_H
#define SHADER_H

#if ANKH2D_WIN32
	#include <GL\glew.h>
	#include <stdint.h>
	#include <string>
	#include "logger.h"

	#include "bmath.h"

	#define VERTEX 0
	#define FRAGMENT 1
	#define NUM_SHADERS 2

	typedef struct
	{
		GLuint program;
		GLuint shaders[NUM_SHADERS];
	} shader;

	shader shader_create(const char* _filename);
	const char* shader_load(const char* _filename);
	void shader_check(GLuint _shader, GLuint _flag, bool _isProgram);
	GLuint shader_get(const char* _buffer, GLenum _shaderType);
	void shader_bind_program(GLuint* _program); 

	void shader_bind_attrib_location(shader* _shader, uint8_t _index, char* _name);
	void shader_set_uniform_mat4(GLuint _uniform, const mat4* _matrix4, bool _transpose);
	void shader_set_uniform_float(GLuint _uniform, float _float);
	void shader_set_uniform_vec2(GLuint _uniform, vec2 _vec2);
	void shader_set_uniform_vec3(GLuint _uniform, vec3 _vec3);
	GLuint shader_get_uniform_location(shader* _shader, char* _uniform);
	void shader_destroy(shader* _shader);
#elif ANKH2D_PSX

#endif

#endif