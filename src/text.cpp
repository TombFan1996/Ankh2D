#include "text.h"

//charSize is how many pixels wide each font letter is
//asciiOffset is incase the font texture does not exactly match the ascii table
//spacing is how many pixels between each of the letters yous want
Text::Text(FT_Library& m_ft, char* _fontName, uint8_t _fontSize, Shader* _shader)
{
	//load the font into a face
	if (FT_New_Face(m_ft, _fontName, 0, &m_face)){
		log_fprint("Could not open %s", _fontName);
	}

	//set height to 48 pixels
	FT_Set_Pixel_Sizes(m_face, 0, _fontSize);

	//create and bind texture
	glGenTextures(1, &m_fontTex);
	glBindTexture(GL_TEXTURE_2D, m_fontTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//has to be disable because glyphs are 1 byte greyscale.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	log_fprint("Disabled 4-byte alignment restriction for Freetype");

	m_shader = _shader;

	m_colour = m_shader->getUniformLocation("colour");

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	m_defaultProj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	//setup default transform
	m_fontColour = glm::vec3(1.0f, 1.0f, 1.0f);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void Text::draw(std::string _text, glm::vec2 _pos, glm::vec2 _size)
{	
	glBindVertexArray(m_VAO);

	//bind our program
	glUseProgram(m_shader->getProgram());

	//set the font colour
	m_shader->setUniformVec3(m_colour, m_fontColour);

	//transparency on the glyphs
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	for (uint32_t i = 0; i < _text.size(); i++)
	{
		FT_Load_Char(m_face, _text[i], FT_LOAD_RENDER);

		//change which texture GL is working w/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fontTex);

		//assign the char in the ttf to the correct glyph
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);

		float x2 = _pos.x + m_face->glyph->bitmap_left * _size.x;
		float y2 = -_pos.y - m_face->glyph->bitmap_top * _size.y;
		float w = m_face->glyph->bitmap.width * _size.x;
		float h = m_face->glyph->bitmap.rows * _size.y;

		GLfloat box[4][4] = {
			// Pos			 // Tex
			x2,		-y2,	 0.0f, 0.0f,
			x2 + w,	-y2,	 1.0f, 0.0f,
			x2,		-y2 - h, 0.0f, 1.0f, 
			x2 + w, -y2 - h, 1.0f, 1.0f,
		};

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		_pos.x += (m_face->glyph->advance.x / 64) * _size.x;
		_pos.y += (m_face->glyph->advance.y / 64) * _size.y;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

Text::~Text()
{
	glDeleteTextures(1, &m_fontTex);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VAO);

	// We Don't Need The Face Information Now That The Display
	FT_Done_Face(m_face);
}