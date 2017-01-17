#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include "graphics.h"
#include "camera.h"
#include "sprite.h"
#include "shader.h"

#include "tmx_parser.h"
#include "tmx_sprite.h"

int main(int argc, char** argv)
{
	//Font rendering in GL
	//http://blogs.agi.com/insight3d/index.php/2008/08/29/rendering-text-fast/
	Graphics graphics("Ankh2D", 1366, 768, false);
	Camera mainCamera(graphics.getWindow());
	Shader shader("assets/sprite");
	
	//create our new sprite
	Sprite darkel("assets/darkel.png", &shader, 
		new Transform(glm::vec2(0, 0), 0.0f));
	 
	//load our map in
	TMX_Parser tmxParser("assets/test_3.tmx");
	//convert map data to a sprite
	TMX_Sprite tmxSprite(tmxParser.getMap(), &shader, new Transform(glm::vec2(600, 300), 0.0f));

	while (!graphics.isClosed())
	{
		//clear the buffer
		graphics.clear();

		tmxSprite.draw(mainCamera.getProjection());

		darkel.update();
		darkel.draw(mainCamera.getProjection());
		
		//update and draw content here
		mainCamera.update(darkel);
		
		//swap the buffers
		graphics.update();
	}

	return 0;
}