#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include "graphics.h"
#include "camera.h"
#include "text.h"
#include "sprite.h"
#include "shader.h"

#include "tmx_parser.h"
#include "tmx_sprite.h"

#include "bmp.h"

int main(int argc, char** argv)
{
	graphics main_graphics = graphics_create("Ankh2D", 1366, 768, false);
	camera main_camera = camera_create();
	shader* spriteShader = shader_create("assets/sprite");
	shader_bind_attrib_location(spriteShader, 0, "model");
	shader_bind_attrib_location(spriteShader, 1, "projection");
	
	//set the alpha value to be BLACK
	bmp_set_alpha_color(vec3_create(0.0f, 0.0f, 0.0f));

	camera_create();

	shader* textShader = shader_create("assets/text");
	shader_bind_attrib_location(textShader, 0, "colour");
	shader_bind_attrib_location(textShader, 1, "model");
	shader_bind_attrib_location(textShader, 2, "projection");
	shader_bind_attrib_location(textShader, 3, "char_index");

	text* newText = text_create("arial", textShader, transform_create(vec2_create(0.0f, 0.0f), 0.0f, vec2_create(10.0f, 10.0f)));
	text_set_colour(newText, vec3_create(1.0f, 0.0f, 0.0f));

	sprite* newSprite = sprite_create("assets/darkel.bmp", spriteShader, 
		transform_create(vec2_create(0.0f, 0.0f), 0.0f, vec2_create(50.0f, 50.0f)));
	
	tmx_sprite* tmx_sprite_1 = tmx_sprite_create("test_2.bin", spriteShader, 
		transform_create(vec2_create(0.0f, 0.0f), 0.0f, vec2_create(50.0f, 50.0f)));

	while (!main_graphics.closed)
	{
		//clear the buffer
		graphics_clear();

		sprite_update(newSprite);
		camera_update(&main_camera, newSprite);
		 
		tmx_sprite_draw(tmx_sprite_1, main_camera.projection);
		sprite_draw(newSprite, main_camera.projection);

		text_draw("TEST LEVEL", newText, vec2_create(40.0f, 40.0f));

		//swap the buffers
		graphics_update(&main_graphics);
	}

	shader_destroy(spriteShader);
	shader_destroy(textShader);
	sprite_destroy(newSprite);
	text_destroy(newText);
	tmx_sprite_destroy(tmx_sprite_1);
	graphics_destroy(&main_graphics);

	return 0;
}