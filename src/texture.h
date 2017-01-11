#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <GL\glew.h>

#include "stb_image.h"

#include <cassert>
#include "logger.h"

class Texture2D
{
	public:
		Texture2D(char* _filename);
		void bind();
		~Texture2D();
	private:
		GLuint m_texture;

};

#endif