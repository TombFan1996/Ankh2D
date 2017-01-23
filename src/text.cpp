#include "text.h"

Text::Text(FT_Library& m_ft, char* _fontName, uint8_t _fontSize, Shader* _shader)
{
	m_shader = _shader;
	m_colour = m_shader->getUniformLocation("colour");

	//setup default font colour
	m_fontColour = glm::vec3(1.0f, 1.0f, 1.0f);

	//load the font into a face
	if (FT_New_Face(m_ft, _fontName, 0, &m_face)){
		log_fprint("Could not open %s", _fontName);
	}

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	m_defaultProj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	m_width = width;
	m_height = height;

	//set height to 48 pixels
	FT_Set_Pixel_Sizes(m_face, 0, _fontSize);

	//store all 128 characters upfront
	this->createText();

	//transparency on the glyphs
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::createText()
{
	for (uint8_t i = 0; i < ASCII_SET_SIZE; i++)
	{
		//create and bind texture
		glGenTextures(1, &m_characters[i].fontTex);
		glBindTexture(GL_TEXTURE_2D, m_characters[i].fontTex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//has to be disable because glyphs are 1 byte greyscale.
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		log_fprint("Disabled 4-byte alignment restriction for Freetype");

		//load the characters
		FT_Load_Char(m_face, (char)i, FT_LOAD_RENDER);

		m_characters[i].bitmap_height = m_face->glyph->bitmap.rows;
		m_characters[i].bitmap_width = m_face->glyph->bitmap.width;
		m_characters[i].bitmap_left = m_face->glyph->bitmap_left;
		m_characters[i].bitmap_top = m_face->glyph->bitmap_top;

		m_characters[i].bitmap_advanceX = m_face->glyph->advance.x;
		m_characters[i].bitmap_advanceY = m_face->glyph->advance.y;

		//assign the char in the ttf to the correct glyph
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// We Don't Need The Face Information Now That The Display
	FT_Done_Face(m_face);
}

//improve drawing by binding all textures onto 1 big quad (1 draw call)
//position coords become uv coords and the quad is the size of all letters combined.

void Text::draw(std::string _text, glm::vec2 _pos)
{	
	glBindVertexArray(m_VAO);
	//bind our program
	glUseProgram(m_shader->getProgram());
	//set the font colour
	m_shader->setUniformVec3(m_colour, m_fontColour);

	glActiveTexture(GL_TEXTURE0);
	
	for (uint32_t i = 0; i < _text.size(); i++)
	{
		//change which texture GL is working w/
		uint8_t id = (uint8_t)_text[i];
		glBindTexture(GL_TEXTURE_2D, m_characters[id].fontTex);

		float sx = 2.0 / m_width;
		float sy = 2.0 / m_height;

		float x2 = _pos.x + m_characters[id].bitmap_left * sx;
		float y2 = -_pos.y - m_characters[id].bitmap_top * sy;

		float w = m_characters[id].bitmap_width * sx;
		float h = m_characters[id].bitmap_height * sy;

		GLfloat box[4][4] = {
			// Pos			  // Tex
			x2,		-y2,	  0.0f, 0.0f,
			x2 + w,	-y2,	  1.0f, 0.0f,
			x2,		-y2 - h,  0.0f, 1.0f, 
			x2 + w, -y2 - h,  1.0f, 1.0f,
		};

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		_pos.x += (m_characters[id].bitmap_advanceX / 64) * sx;
	}

	glBindVertexArray(0);
}

Text::~Text()
{
	glDeleteTextures(ASCII_SET_SIZE, &m_characters[0].fontTex);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);
}