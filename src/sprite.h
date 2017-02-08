#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include "bmath.h"
#include "transform.h"
#include "texture2d.h"
#include "shader.h"

#include "tmx_parser.h"

typedef struct
{
	float speed;
	const uint8_t* keys;
	GLuint model, projection;
	shader* shader;
	transform transform;
	texture2d* texture;
	GLuint vao, vbo;
} sprite;

sprite* sprite_create(const char* _name, shader* _shader, transform _trans);
void sprite_update(sprite* _sprite);
void sprite_draw(sprite* _sprite, mat4 _projection);
void sprite_set_texture(sprite* _sprite, texture2d* _tex);
bool sprite_check_map_collision(tmx_map* _tmx_map, vec2 _sprite_pos);
void sprite_destroy(sprite* _sprite);

#endif