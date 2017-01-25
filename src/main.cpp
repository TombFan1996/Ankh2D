#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include "graphics.h"
#include "camera.h"
#include "text.h"
#include "sprite.h"
#include "shader.h"

#include "tmx_parser.h"
#include "tmx_sprite.h"

//grapics handler = mainGraphics
//camera handler = mainCamera;

int main(int argc, char** argv)
{
	FT_Library ft_lib;

	//setup the lib
	if (FT_Init_FreeType(&ft_lib)){
		log_fprint("Could not init freetype library!");
	}

	graphics_create("Ankh2D C Conversion", 1366, 768, false);
	shader* spriteShader = shader_create("assets/sprite");
	shader_bindAttribLocation(spriteShader, 0, "model");
	shader_bindAttribLocation(spriteShader, 1, "projection");

	camera_create();

	shader* textShader = shader_create("assets/text");
	shader_bindAttribLocation(textShader, 0, "colour");

	text* newText = text_create(ft_lib, "assets/arial.ttf", 24.0f, textShader);
	text_setColour(newText, glm::vec3(1.0f, 0.0f, 0.0f));

	transform spriteTrans;
	spriteTrans.position = glm::vec2(0.0f, 0.0f);
	spriteTrans.rotation = 0.0f;
	spriteTrans.scale = glm::vec2(40.0f, 40.0f);
	sprite* newSprite = sprite_create("assets/darkel.png", spriteShader, &spriteTrans);
	
	tmx_map* map = tmx_parser_create("assets/test_1.tmx");
	transform mapTrans;
	mapTrans.position = glm::vec2(0.0f, 0.0f);
	mapTrans.rotation = 0.0f;
	mapTrans.scale = glm::vec2(40.0f, 40.0f);
	tmx_sprite* tmx_sprite = tmx_sprite_create(map, spriteShader, &mapTrans);

	while (!mainGraphics->closed)
	{
		//clear the buffer
		graphics_clear();

		tmx_sprite_draw(tmx_sprite, mainCamera->projection);
		text_draw(newText, "It Works in C!", glm::vec2(-1 + 8 * (2.0f / mainGraphics->width), 
			1 - 50 * (2.0f / mainGraphics->height)));

		sprite_update(newSprite);
		sprite_draw(newSprite, mainCamera->projection);

		camera_update(newSprite);

		//swap the buffers
		graphics_update();
	}

	camera_destroy();
	
	shader_destroy(spriteShader);
	shader_destroy(textShader);
	
	sprite_destroy(newSprite);

	text_destroy(newText);

	tmx_sprite_destroy(tmx_sprite);
	tmx_parser_destroy(map);
	
	graphics_destroy();

	//remove freetype
	FT_Done_FreeType(ft_lib);

	return 0;
}