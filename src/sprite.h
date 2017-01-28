#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <glm\vec2.hpp>
#include "transform.h"
#include "texture2d.h"
#include "shader.h"

typedef struct
{
	float speed;
	const uint8_t* keys;
	GLuint model, projection;
	shader* shader;
	transform* transform;
	texture2d* texture;
	GLuint vao, vbo;
} sprite;

sprite* sprite_create(const char* _name, shader* _shader, transform* _trans);
void sprite_update(sprite* _sprite);
void sprite_draw(sprite* _sprite, glm::mat4 _projection);
void sprite_setTexture(sprite* _sprite, texture2d* _tex);
void sprite_destroy(sprite* _sprite);

#endif