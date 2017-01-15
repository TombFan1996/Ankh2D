#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include "graphics.h"
#include "sprite.h"
#include "shader.h"

#include "tmx_parser.h"
#include "tmx_sprite.h"

int main(int argc, char** argv)
{
	//Font rendering in GL
	//http://blogs.agi.com/insight3d/index.php/2008/08/29/rendering-text-fast/
	Graphics graphics("Ankh2D", 1366, 768, false);
	Shader shader("assets/sprite", graphics.getWidth(), graphics.getHeight());
	
	//create our new sprite
	Sprite darkel("assets/darkel.png", &shader, new Transform(glm::vec2(0,0), 0.0f));
	 
	TMX_Parser tmxParser("assets/test_tiny_3.tmx");
	TMX_Sprite tmxSprite(tmxParser.getMap(), &shader, new Transform(glm::vec2(600, 300), 0.0f, glm::vec2(100,100)));

	while (!graphics.isClosed())
	{
		//clear the buffer
		graphics.clear();
		//update and draw content here
		darkel.draw();
		tmxSprite.draw();
		//swap the buffers
		graphics.update();
	}

	return 0;
}