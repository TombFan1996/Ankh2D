#include "text.h"

//charSize is how many pixels wide each font letter is
//asciiOffset is incase the font texture does not exactly match the ascii table
//spacing is how many pixels between each of the letters yous want
Text::Text(char* _fontName, glm::vec2 _charSize, uint8_t _asciiOffset, float _spacing, Shader* _shader)
{
	m_characterSize = _charSize;
	m_asciiOffset = _asciiOffset;
	m_spacing = _spacing;
	m_shader = _shader;

	m_model = m_shader->getUniformLocation("model");
	m_projection = m_shader->getUniformLocation("projection");
	m_colour = m_shader->getUniformLocation("colour");

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	m_defaultProj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	//setup default transform
	m_transform = new Transform(glm::vec2(0,0), 0, glm::vec2(50, 50));
	m_fontColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_font = new Texture2D(_fontName);

	//normalised coords for sizing
	m_textNormalX = (1.0f / m_font->getWidth()) * m_characterSize.x;
	m_textNormalY = (1.0f / m_font->getHeight()) * m_characterSize.y;

	//number of tiles in the map
	m_numTextX = m_font->getWidth() / m_characterSize.x;
	m_numTextY = m_font->getHeight() / m_characterSize.y;

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//6 (2 triangles) * 4 (2 pos, 2 tex)
	//nothing to pass in yet
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);  
}

void Text::draw(std::string _text, glm::vec2 _pos)
{
	m_transform->setPosition(_pos);
	
	glBindVertexArray(m_VAO);

	//bind our program
	glUseProgram(m_shader->getProgram());

	//communicate w/ uniforsms
	//send the model matrix off
	m_shader->setUniformMat4(m_model, m_transform->getModelMatrix());

	//send the projection matrix off
	m_shader->setUniformMat4(m_projection, m_defaultProj);

	//set the font colour
	m_shader->setUniformVec3(m_colour, m_fontColour);

	//bind our texture
	m_font->bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	for (uint32_t i = 0; i < _text.size(); i++)
	{
		//get the ascii equivalent of the char
		uint8_t id = (uint8_t)_text[i] - m_asciiOffset;

		//get x and y coord via mod and div
		uint16_t idCoordX = id % m_numTextX;
		uint16_t idCoordY = id / m_numTextY;

		//nothing Y-axis related needs to be done in relation to positioning
		float posX = i * 1.0f;

		GLfloat vertices[] = {
			// Pos					   // Tex
			0.0f + posX, 1.0f, (m_textNormalX * idCoordX), (m_textNormalY * idCoordY) + m_textNormalY,
			1.0f + posX, 0.0f, (m_textNormalX * idCoordX) + m_textNormalX, (m_textNormalY * idCoordY),
			0.0f + posX, 0.0f, (m_textNormalX * idCoordX), (m_textNormalY * idCoordY), 
   
			0.0f + posX, 1.0f, (m_textNormalX * idCoordX), (m_textNormalY * idCoordY) + m_textNormalY,
			1.0f + posX, 1.0f, (m_textNormalX * idCoordX) + m_textNormalX, (m_textNormalY * idCoordY) + m_textNormalY,
			1.0f + posX, 0.0f, (m_textNormalX * idCoordX) + m_textNormalX, (m_textNormalY * idCoordY)
		};

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);  

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Text::~Text()
{

}