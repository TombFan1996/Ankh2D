#ifndef SPRITE_H
#define SPRITE_H

//#include <SDL.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "bmath.h"
#include "tmx_sprite.h"
#include "transform.h"
#include "texture2d.h"
#include "shader.h"

typedef struct
{
	float speed;
	const uint8_t* keys;
	GLuint model, projection;
	shader* shader;
	transform transform;
	vec2 old_pos;
	texture2d* texture;
	GLuint vao, vbo;
	GLFWwindow* window;
} sprite;

sprite* sprite_create(const char* _name, shader* _shader, transform _trans, GLFWwindow* _window);
bool sprite_update(sprite* _sprite);
void sprite_draw(sprite* _sprite, mat4* _projection);
void sprite_set_texture(sprite* _sprite, texture2d* _tex);
void sprite_map_intersect(tmx_sprite* _tmx_map, sprite* _sprite, bool _sprite_update);
void sprite_destroy(sprite* _sprite);

#endif