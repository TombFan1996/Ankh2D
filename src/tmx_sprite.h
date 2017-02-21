#ifndef TMX_RENDERER_H
#define TMX_RENDERER_H

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
		shader shader;
		transform transform;
		uint8_t num_layers;
		uint32_t num_tiles;
		GLuint vao, vbo;
		texture2d tilemap;
		tmx_map* map;
	} tmx_sprite;

	tmx_sprite tmx_sprite_create(const char* _mapName, shader* _shader, transform _trans);
	void tmx_sprite_draw(mat4* _projection, tmx_sprite* _sprite);
	void tmx_sprite_destroy(tmx_sprite* _sprite);

#elif ANKH2D_PSX

#endif

#endif