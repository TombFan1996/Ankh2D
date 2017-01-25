#include "text.h"

void freetype_init(FT_Library* _ftlib)
{
	//setup the lib
	if (FT_Init_FreeType(_ftlib)){
		log_fprint("Could not init freetype library!");
	}
}

void freetype_deinit(FT_Library* _ftlib)
{
	//remove freetype
	FT_Done_FreeType(*_ftlib);
}

text* text_create(FT_Library& m_ft, const char* _fontName, uint8_t _fontSize, shader* _shader)
{
	text* newText = new text;
	newText->shader = _shader;
	newText->colour = shader_getUniformLocation(newText->shader, "colour");

	//setup default font colour
	newText->fontColour = glm::vec3(1.0f, 1.0f, 1.0f);

	//load the font into a face
	if (FT_New_Face(m_ft, _fontName, 0, &newText->face)){
		log_fprint("Could not open %s", _fontName);
	}

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	newText->defaultProj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	newText->width = width;
	newText->height = height;

	//transparency on the glyphs
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	//has to be disable because glyphs are 1 byte greyscale.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	log_fprint("Disabled 4-byte alignment restriction for Freetype");

	//set height to 48 pixels
	FT_Set_Pixel_Sizes(newText->face, 0, _fontSize);

	//store all 128 characters upfront
	text_loadCharacters(newText);

	glGenVertexArrays(1, &newText->vao);
	glBindVertexArray(newText->vao);

	glGenBuffers(1, &newText->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newText->vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return newText;
}

void text_loadCharacters(text* _text)
{
	for (uint8_t i = 0; i < ASCII_SET_SIZE; i++)
	{
		//create and bind texture
		glGenTextures(1, &_text->characters[i].fontTex);
		glBindTexture(GL_TEXTURE_2D, _text->characters[i].fontTex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//load the characters
		FT_Load_Char(_text->face, (char)i, FT_LOAD_RENDER);

		_text->characters[i].bitmap_height = _text->face->glyph->bitmap.rows;
		_text->characters[i].bitmap_width = _text->face->glyph->bitmap.width;
		_text->characters[i].bitmap_left = _text->face->glyph->bitmap_left;
		_text->characters[i].bitmap_top = _text->face->glyph->bitmap_top;

		_text->characters[i].bitmap_advanceX = _text->face->glyph->advance.x;
		_text->characters[i].bitmap_advanceY = _text->face->glyph->advance.y;

		//assign the char in the ttf to the correct glyph
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _text->face->glyph->bitmap.width, _text->face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, _text->face->glyph->bitmap.buffer);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// We Don't Need The Face Information Now That The Display
	FT_Done_Face(_text->face);
}

void text_setColour(text* _text, glm::vec3 _colour)
{
	_text->fontColour = _colour;
}

//so pointless considering we unload all of the face after
//we cache it to the character struct
void text_setSize(text* _text, float _size)
{
	FT_Set_Pixel_Sizes(_text->face, 0, _size);
}

//improve drawing by binding all textures onto 1 big quad (1 draw call)
//position coords become uv coords and the quad is the size of all letters combined.
void text_draw(text* _text, std::string _str, glm::vec2 _pos)
{
	glBindVertexArray(_text->vao);
	//bind our program
	glUseProgram(_text->shader->program);
	//set the font colour
	shader_setUniformVec3(_text->colour, _text->fontColour);

	glActiveTexture(GL_TEXTURE0);
	
	for (uint32_t i = 0; i < _str.size(); i++)
	{
		//change which texture GL is working w/
		uint8_t id = (uint8_t)_str[i];
		glBindTexture(GL_TEXTURE_2D, _text->characters[id].fontTex);

		float sx = 2.0 / _text->width;
		float sy = 2.0 / _text->height;

		float x2 = _pos.x + _text->characters[id].bitmap_left * sx;
		float y2 = -_pos.y - _text->characters[id].bitmap_top * sy;

		//the width of each character
		float w = _text->characters[id].bitmap_width * sx;
		float h = _text->characters[id].bitmap_height * sy;

		GLfloat box[4][4] = {
			// Pos			  // Tex
			x2,		-y2,	  0.0f, 0.0f,
			x2 + w,	-y2,	  1.0f, 0.0f,
			x2,		-y2 - h,  0.0f, 1.0f, 
			x2 + w, -y2 - h,  1.0f, 1.0f,
		};

		glBindBuffer(GL_ARRAY_BUFFER, _text->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		_pos.x += (_text->characters[id].bitmap_advanceX / 64) * sx;
	}

	glBindVertexArray(0);
}

void text_destroy(text* _text)
{
	glDeleteTextures(ASCII_SET_SIZE, &_text->characters[0].fontTex);

	shader_destroy(_text->shader);

	glDeleteBuffers(1, &_text->vbo);
	glDeleteBuffers(1, &_text->vao);
}