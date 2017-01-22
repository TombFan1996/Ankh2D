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

class Text
{
	public:
		Text(FT_Library& m_ft, char* _fontName, uint8_t _fontSize, Shader* _shader);
		void setColour(glm::vec3 _colour){m_fontColour = _colour;}
		void draw(std::string _text, glm::vec2 _pos, glm::vec2 _size);
		~Text();
	private:
		GLuint m_fontTex;
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