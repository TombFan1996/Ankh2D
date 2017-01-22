#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include "graphics.h"
#include "camera.h"
#include "text.h"
#include "sprite.h"
#include "shader.h"

#include "tmx_parser.h"
#include "tmx_sprite.h"

int main(int argc, char** argv)
{
	FT_Library ft_lib;

	//setup the lib
	if (FT_Init_FreeType(&ft_lib)){
		log_fprint("Could not init freetype library!");
	}

	//Font rendering in GL
	//http://blogs.agi.com/insight3d/index.php/2008/08/29/rendering-text-fast/
	Graphics graphics("Ankh2D", 1366, 768, false);
	Camera mainCamera;

	Shader spriteShader("assets/sprite");
	spriteShader.bindAttribLocation(0, "model");
	spriteShader.bindAttribLocation(1, "projection");

	Shader textShader("assets/text");
	textShader.bindAttribLocation(0, "colour");

	Text* newText = new Text(ft_lib, "assets/arial.ttf", 24.0f, &textShader);
	newText->setColour(glm::vec3(1.0f, 0.0f, 0.0f));

	//create our new sprite
	Sprite darkel("assets/darkel.png", &spriteShader, 
		new Transform(glm::vec2(0, 0), 0.0f, glm::vec2(300,300)));
	 
	//load our map in
	TMX_Parser tmxParser("assets/test_1.tmx");
	//convert map data to a sprite
	TMX_Sprite tmxSprite(tmxParser.getMap(), &spriteShader, new Transform(glm::vec2(600, 300), 0.0f));

	while (!graphics.isClosed())
	{
		//clear the buffer
		graphics.clear();

		tmxSprite.draw(mainCamera.getProjection());

		darkel.update();
		darkel.draw(mainCamera.getProjection());

		float sx = 2.0 / graphics.getWidth();
		float sy = 2.0 / graphics.getHeight();

		newText->draw("It works!", glm::vec2(-1 + 8 * sx, 1 - 50 * sy), glm::vec2(sx, sy));
		
		//update and draw content here
		mainCamera.update(darkel);

		//swap the buffers
		graphics.update();
	}

	//needs to be called before destruction of freetype
	delete newText;

	//remove freetype
	FT_Done_FreeType(ft_lib);

	return 0;
}