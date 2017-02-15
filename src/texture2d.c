#include "texture2d.h"

texture2d* texture2d_create(const char* _filename)
{
	texture2d* new_tex = (texture2d*)malloc(sizeof(texture2d));

	BMP_24* texture = bmp_24_load(_filename);
	RGBA_DATA* texture_rgba = bmp_24_bgr_rgba(texture);

	new_tex->width = texture->ih.width;
	new_tex->height = texture->ih.height;

	//create and bind texture
	glGenTextures(1, &new_tex->texture);
	glBindTexture(GL_TEXTURE_2D, new_tex->texture);

	//if we read outside the texture size, it will start again (wrap repeat)
	//use GL_CLAMP for a default colour to be used outside bounds of tex
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//if texture is bigger than specified or rotate, linearly interpolate
	//GL_NEAREST give no texture filtering.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//construct the texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_tex->width, new_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_rgba); 

	free(texture_rgba);
	free(texture);

	log_fprint("'%s' successfully created", _filename);

	return new_tex;
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