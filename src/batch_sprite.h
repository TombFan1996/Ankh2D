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
		shader* shader;
		transform* transform;
		GLuint vao, vbo;
		texture2d tilemap;
	} batch_sprite;

	batch_sprite batch_sprite_create(const char* _sprite_name, uint8_t _tilesize, shader* _shader, transform _trans);
	void batch_sprite_draw(uint8_t _draw_id, mat4* _projection, batch_sprite* _sprite);
	void batch_sprite_destroy(batch_sprite* _sprite);

#elif ANKH2D_PSX

#endif

#endif