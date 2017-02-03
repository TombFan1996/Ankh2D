#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <fstream>
#include <stdint.h>
#include <string>
#include "logger.h"

#include "math.h"

#define VERTEX 0
#define FRAGMENT 1
#define NUM_SHADERS 2

typedef struct
{
	GLuint program;
	GLuint shaders[NUM_SHADERS];
} shader;

shader* shader_create(const char* _filename);
const char* shader_load(const char* _filename);
void shader_check(GLuint _shader, GLuint _flag, bool _isProgram);
GLuint shader_get(const char* _buffer, GLenum _shaderType);

void shader_bindAttribLocation(shader* _shader, uint8_t _index, char* _name);
void shader_setUniformMat4(GLuint _uniform, mat4 _matrix4);
void shader_setUniformFloat(GLuint _uniform, float _float);
void shader_setUniformVec2(GLuint _uniform, vec2 _vec2);
void shader_setUniformVec3(GLuint _uniform, vec3 _vec3);
GLuint shader_getUniformLocation(shader* _shader, char* _uniform);
void shader_destroy(shader* _shader);

#endif