#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <vector>
#include <SDL.h>
#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\gtx\transform.hpp>

#include "logger.h"
#include "shader.h"
#include "transform.h"
#include "texture2d.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define ASCII_SET_SIZE 128

typedef struct
{
	GLuint fontTex;
	uint16_t bitmap_width;
	uint16_t bitmap_height;
	uint16_t bitmap_left;
	uint16_t bitmap_top;

	float bitmap_advanceX;
	float bitmap_advanceY;
} character;

typedef struct
{
	uint16_t width, height;
	character characters[ASCII_SET_SIZE];
	FT_Face face;
	glm::mat4 defaultProj;
	GLuint vao, vbo;
	GLuint colour;
	shader* shader;
	glm::vec3 fontColour;
	float textNormalX, textNormalY;
	uint16_t numTextX, numTextY;
} text;

void freetype_init(FT_Library* _ftlib);
void freetype_deinit(FT_Library* _ftlib);

text* text_create(FT_Library& m_ft, const char* _fontName, uint8_t _fontSize, shader* _shader);
void text_loadCharacters(text* _text);
void text_setColour(text* _text, glm::vec3 _colour);
void text_setSize(text* _text, float _size);
void text_draw(text* _text, std::string _str, glm::vec2 _pos);
void text_destroy(text* _text);

#endif