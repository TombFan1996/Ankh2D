#ifndef SPRITE_H
#define SPRITE_H

#if ANKH2D_WIN32
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>

	#include "bmath.h"
	#include "tmx_sprite.h"
	#include "transform.h"
	#include "texture2d.h"
	#include "shader.h"
	#include "btime.h"

	typedef struct
	{
		float speed;
		GLuint model, projection;
		shader* shader;
		transform transform;
		vec2 old_pos;
		texture2d* texture;
		GLuint vao, vbo;
		GLFWwindow* window;
	} sprite;

	sprite sprite_create(texture2d* _texture, shader* _shader, transform _trans, GLFWwindow* _window);
	bool sprite_update(sprite* _sprite, btime* _time);
	void sprite_draw(mat4* _projection, sprite* _sprite);
	void sprite_set_texture(sprite* _sprite, texture2d* _tex);
	void sprite_map_intersect(tmx_sprite* _tmx_map, sprite* _sprite, bool _sprite_update);
	void sprite_destroy(sprite* _sprite);

#elif ANKH2D_PSX

	#include "types.h"
	#include <libgte.h>
	#include <libgpu.h>
	#include <libetc.h>
	#include <libgs.h>

	#include "transform.h"
	#include "texture2d.h"

	typedef struct
	{
		//tim image struct
		texture2d texture;
		//sprite based on tim_image
		GsSPRITE gs_sprite;
		//the position of the sprite (int16_vec2)
		transform transform;
	} sprite;

	void sprite_create(sprite* _sprite, uint32_t* _tpage, uint32_t* _clut);
	void sprite_draw(sprite* _sprite, GsOT* _ot);
	
#endif

#endif