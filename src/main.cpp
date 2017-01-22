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
	//Font rendering in GL
	//http://blogs.agi.com/insight3d/index.php/2008/08/29/rendering-text-fast/
	Graphics graphics("Ankh2D", 1366, 768, false);
	Camera mainCamera;

	Shader spriteShader("assets/sprite");
	spriteShader.bindAttribLocation(0, "model");
	spriteShader.bindAttribLocation(1, "projection");

	Shader textShader("assets/text");
	textShader.bindAttribLocation(0, "model");
	textShader.bindAttribLocation(1, "projection");
	textShader.bindAttribLocation(2, "colour");

	Text newText("assets/font.png", glm::vec2(15, 12), 22, 1.0f, &textShader);
	newText.setColour(glm::vec3(0.25f, 0.0f, 0.0f));

	//create our new sprite
	Sprite darkel("assets/darkel.png", &spriteShader, 
		new Transform(glm::vec2(0, 0), 0.0f));
	 
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

		newText.draw("test", glm::vec2(10, 10));
		
		//update and draw content here
		mainCamera.update(darkel);

		//swap the buffers
		graphics.update();
	}

	return 0;
}