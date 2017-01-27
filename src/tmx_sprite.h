#ifndef TMX_RENDERER_H
#define TMX_RENDERER_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "texture2d.h"
#include "transform.h"
#include "tmx_parser.h"

#include "shader.h"

//WARNING: tilemap of equal width and height only!
//the offset vs tile size is not sorted yet

typedef struct
{
	GLuint model, projection;
	shader* shader;
	transform* transform;
	uint32_t numTiles;
	GLuint vao, vbo;
	texture2d* tilemap;
	tmx_map* map;
} tmx_sprite;

tmx_sprite* tmx_sprite_create(const char* _mapName, shader* _shader, transform* _trans);
void tmx_sprite_draw(tmx_sprite* _sprite, glm::mat4 _projection);
void tmx_sprite_destroy(tmx_sprite* _sprite);

#endif