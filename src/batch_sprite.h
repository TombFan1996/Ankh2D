#ifndef BATCH_SPRITE_H
#define BATCH_SPRITE_H

#if ANKH2D_WIN32
	#include <GL\glew.h>

	#include "bmath.h"
	#include "texture2d.h"
	#include "transform.h"
	#include "tmx_parser.h"
	#include "shader.h"

	typedef struct
	{
		GLuint model, projection;
		transform transform;
		GLuint vao, vbo;
	} batch_sprite;

	batch_sprite batch_sprite_create(int32_vec2 _pixel_coord, vec2 _tile_size, texture2d* _tex, shader* _shader, transform _trans);
	void batch_sprite_draw(mat4* _projection, batch_sprite* _sprite);
	void batch_sprite_destroy(batch_sprite* _sprite);

#elif ANKH2D_PSX

#endif

#endif