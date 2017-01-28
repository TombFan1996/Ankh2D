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

#define ASCII_SET_SIZE 128

//http://www.angelcode.com/products/bmfont/doc/file_format.html

//arial.bmp
#define FONT_SIZE_PLUS_TERMINATOR 10

typedef struct
{
	int16_t font_size;
	char bitfield; //bit 0: smooth, bit 1: unicode, bit 2: italic, bit 3: bold, bit 4: fixed height, bits 5-7: reserved
	uint8_t char_set;
	uint16_t stretch_H;
	uint8_t aa;
	uint8_t padding_up;
	uint8_t padding_right;
	uint8_t padding_left;
	uint8_t spacing_horizontal;
	uint8_t spacing_vertical;
	uint8_t outline; //v2
} FNT_INFO_BLOCK;

typedef struct
{
	uint16_t line_height;
	uint16_t base;
	uint16_t scale_w;
	uint16_t scale_h;
	uint16_t pages;
	char bitfield; //bits 0-6: reserved, bit 7: packed
	uint8_t alpha_channel;
	uint8_t green_channel;
	uint8_t red_channel;
	uint8_t blue_channel;
} FNT_COMMON_BLOCK;

//only contains a char pointer
/*typedef struct
{
	char* pageNames;
} FNT_PAGES_BLOCK;*/

typedef struct
{
	uint32_t unicode_id;
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	int16_t offset_x;
	int16_t offset_y;
	int16_t advantage_x;
	uint8_t page;
	uint8_t chnl;
} FNT_CHAR_BLOCK;

typedef struct
{
	uint32_t first;
	uint32_t second;
	int16_t amount;
} FNT_KERNING_PAIR_BLOCK;

typedef struct
{
	GLuint *vao, *vbo;
	GLuint colour, model, projection, char_index;
	shader* shader;
	glm::vec3 fontColour;

	texture2d* texture;

	FNT_INFO_BLOCK info_block;
	char* filename, *page_names;
	uint16_t num_char_block;
	FNT_CHAR_BLOCK* char_block;
	FNT_COMMON_BLOCK common_block;
	FNT_KERNING_PAIR_BLOCK kerning_pair_block;

	transform* transform;
	glm::mat4 defaultProj;
} text;

text* text_create(const char* _fontName, shader* _shader, transform* _trans);
void text_loadFNT(text* _text, const char* _name);
void text_loadBMP(text* _text, const char* _name);
void text_setColour(text* _text, glm::vec3 _colour);
void text_draw(text* _text, glm::vec2 _pos, std::string _str);
void text_destroy(text* _text);

#endif