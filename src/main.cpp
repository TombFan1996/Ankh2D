#define STB_IMAGE_IMPLEMENTATION

#include <GL\glew.h>

#include "graphics.h"
#include "sprite.h"
#include "shader.h"

int main(int argc, char** argv)
{
	//Font rendering in GL
	//http://blogs.agi.com/insight3d/index.php/2008/08/29/rendering-text-fast/
	Graphics graphics("Ankh2D", 1366, 768, false);
	Shader shaderManager("assets/sprite", graphics.getWidth(), graphics.getHeight());
	
	//create our new sprite
	Sprite newSprite("assets/darkel.png", new Transform(glm::vec2(0,0), 0.0f));
	newSprite.getTransform()->setPosition(glm::vec2(0, 0));

	while (!graphics.isClosed())
	{
		//clear the buffer
		graphics.clear();
		//update our sprites via sprite manager
		shaderManager.update(newSprite);
		//swap the buffers
		graphics.update();
	}

	return 0;
}