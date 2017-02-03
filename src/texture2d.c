#include "texture2d.h"

texture2d* texture2d_create(const char* _filename)
{
	texture2d* newTex = (texture2d*)malloc(sizeof(texture2d));
	
	int width, height, numComponents;
	unsigned char* imageBuffer = stbi_load(_filename, &width, &height, &numComponents, 4);
	newTex->width = width;
	newTex->height = height;

	if (imageBuffer == NULL)
		log_fprint("%s loading has failed!", _filename);

	else
	{
		//create and bind texture
		glGenTextures(1, &newTex->texture);
		glBindTexture(GL_TEXTURE_2D, newTex->texture);

		//if we read outside the texture size, it will start again (wrap repeat)
		//use GL_CLAMP for a default colour to be used outside bounds of tex
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//if texture is bigger than specified or rotate, linearly interpolate
		//GL_NEAREST give no texture filtering.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//construct the texture image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTex->width, newTex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer); 
	}

	stbi_image_free(imageBuffer);

	return newTex;
}

void texture2d_bind(texture2d* _tex)
{
	//change which texture GL is working w/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex->texture);
}

void texture2d_destroy(texture2d* _tex)
{
	glDeleteTextures(1, &_tex->texture);
}