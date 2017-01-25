#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <GL\glew.h>

#include "stb_image.h"

#include <cassert>
#include "logger.h"

typedef struct
{
	uint16_t width, height;
	GLuint texture;
} texture2d;

texture2d* texture2d_create(const char* _filename);
void texture2d_bind(texture2d* _tex);
void texture2d_destroy(texture2d* _tex);

#endif