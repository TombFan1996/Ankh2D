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
	graphics_create("Ankh2D C Conversion", 1366, 768, false);
	shader* spriteShader = shader_create("assets/sprite");
	shader_bindAttribLocation(spriteShader, 0, "model");
	shader_bindAttribLocation(spriteShader, 1, "projection");

	camera_create();

	shader* textShader = shader_create("assets/text");
	shader_bindAttribLocation(textShader, 0, "colour");
	shader_bindAttribLocation(textShader, 1, "model");
	shader_bindAttribLocation(textShader, 2, "projection");
	shader_bindAttribLocation(textShader, 3, "char_index");

	text* newText = text_create("arial", textShader, transform_create(glm::vec2(50.0f, 10.0f), 0.0f, glm::vec2(15.0f, 15.0f)));
	text_setColour(newText, glm::vec3(1.0f, 1.0f, 1.0f));

	sprite* newSprite = sprite_create("assets/darkel.png", spriteShader, 
		transform_create(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(40.0f, 40.0f)));
	
	tmx_sprite* tmx_sprite_1 = tmx_sprite_create("test_1.tmx", spriteShader, 
		transform_create(glm::vec2(0.0f, 0.0f)));

	tmx_sprite* tmx_sprite_2 = tmx_sprite_create("test_2.tmx", spriteShader, 
		transform_create(glm::vec2(300.0f, 0.0f)));

	while (!mainGraphics->closed)
	{
		//clear the buffer
		graphics_clear();

		tmx_sprite_draw(tmx_sprite_1, mainCamera->projection);
		tmx_sprite_draw(tmx_sprite_2, mainCamera->projection);
		
		text_draw(newText, glm::vec2(10.0f, 10.0f), "11111");

		//sprite_update(newSprite);
		sprite_draw(newSprite, mainCamera->projection);

		camera_update(mainCamera);

		//swap the buffers
		graphics_update();
	}

	camera_destroy();
	shader_destroy(spriteShader);
	shader_destroy(textShader);
	sprite_destroy(newSprite);
	text_destroy(newText);
	tmx_sprite_destroy(tmx_sprite_1);
	tmx_sprite_destroy(tmx_sprite_2);
	graphics_destroy();

	return 0;
}