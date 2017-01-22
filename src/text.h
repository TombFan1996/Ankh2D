#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <vector>
#include <SDL.h>
#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\gtx\transform.hpp>

#include "shader.h"
#include "transform.h"
#include "texture2d.h"

class Text
{
	public:
		Text(char* _fontName, glm::vec2 _charSize, uint8_t _asciiOffset, float _spacing, Shader* _shader);
		Transform* getTransform(){return m_transform;}
		void setColour(glm::vec3 _colour){m_fontColour = _colour;}
		void draw(std::string _text, glm::vec2 _pos);
		~Text();
	private:
		Transform* m_transform;
		glm::mat4 m_defaultProj;
		GLuint m_VAO, m_VBO;
		Texture2D* m_font;
		GLuint m_model, m_projection, m_colour;
		Shader* m_shader;
		glm::vec2 m_characterSize;
		glm::vec3 m_fontColour;
		float m_spacing;
		uint8_t m_asciiOffset;

		float m_textNormalX, m_textNormalY;
		uint16_t m_numTextX, m_numTextY;
};

#endif