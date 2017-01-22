#include "texture2d.h"

Texture2D::Texture2D(const char* _filename)
{
	int width, height, numComponents;
	unsigned char* imageBuffer = stbi_load(_filename, &width, &height, &numComponents, 4);
	m_width = width;
	m_height = height;

	if (imageBuffer == NULL)
		log_fprint("%s loading has failed!", _filename);

	else
	{
		//create and bind texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		//if we read outside the texture size, it will start again (wrap repeat)
		//use GL_CLAMP for a default colour to be used outside bounds of tex
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//if texture is bigger than specified or rotate, linearly interpolate
		//GL_NEAREST give no texture filtering.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//construct the texture image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer); 
	}

	stbi_image_free(imageBuffer);
}

//can bind MAX of 32 texs
void Texture2D::bind()
{
	//change which texture GL is working w/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_texture);
}
