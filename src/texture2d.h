#ifndef TEXTURE_H
#define TEXTURE_H

#if ANKH2D_WIN32
	#include <GL\glew.h>
	#include <cassert>

	#include "bmath.h"
	#include "bmp.h"

	#include "logger.h"

	typedef struct
	{
		uint16_t width, height;
		GLuint texture;
	} texture2d;

	void texture2d_alpha_colour(vec3 _alphaCol);
	texture2d* texture2d_create(const char* _filename);
	void texture2d_bind(texture2d* _tex);
	void texture2d_destroy(texture2d* _tex);

#elif ANKH2D_PSX

#endif

#endif