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
		uint16_t getWidth(){return m_width;}
		uint16_t getHeight(){return m_height;}
		~Texture2D();
	private:
		uint16_t m_width, m_height;
		GLuint m_texture;

};

#endif