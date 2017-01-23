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
} Char;

class Text
{
	public:
		Text(FT_Library& m_ft, char* _fontName, uint8_t _fontSize, Shader* _shader);
		void createText();
		void setColour(glm::vec3 _colour){m_fontColour = _colour;}
		void setSize(uint8_t _fontSize){FT_Set_Pixel_Sizes(m_face, 0, _fontSize);}
		void draw(std::string _text, glm::vec2 _pos);
		~Text();
	private:
		uint16_t m_width, m_height;
		Char m_characters[ASCII_SET_SIZE];
		FT_Face m_face;
		glm::mat4 m_defaultProj;
		GLuint m_VAO, m_VBO;
		GLuint m_colour;
		Shader* m_shader;
		glm::vec3 m_fontColour;
		float m_textNormalX, m_textNormalY;
		uint16_t m_numTextX, m_numTextY;
};

#endif