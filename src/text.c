#include "text.h"

text* text_create(const char* _fontPath, shader* _shader, transform _trans)
{
	text* new_text = (text*)malloc(sizeof(text));

	new_text->shader = _shader;
	new_text->colour = shader_get_uniform_location(new_text->shader, "colour");
	new_text->model = shader_get_uniform_location(new_text->shader, "model");
	new_text->projection = shader_get_uniform_location(new_text->shader, "projection");
	new_text->char_index = shader_get_uniform_location(new_text->shader, "char_index");
	
	new_text->transform = _trans;

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	//SSE = __m128 (16 byte bound), float[4][4] also 16 bytes
	new_text->default_proj = (mat4*)malloc(sizeof(mat4));
	mat4_orthographic(new_text->default_proj, 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	//setup default font colour
	new_text->font_colour = vec3_create(1.0f, 1.0f, 1.0f);

	//transparency on the font
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	//load the fnt file in binary
	text_load_fnt(new_text, _fontPath);
	text_load_bmp(new_text, new_text->page_names);

	return new_text;
}

void text_load_bmp(text* _text, const char* _name)
{
	std::string name;
	name += "./assets/";
	name += _name;
	_text->texture = texture2d_create(name.c_str());
	texture2d_bind(_text->texture);

	_text->vao = (GLuint*)malloc(sizeof(GLuint) * _text->num_char_block);
	_text->vbo = (GLuint*)malloc(sizeof(GLuint) * _text->num_char_block);

	/*GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 
    
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};*/

	//normalised uv coords now
	float x_width = 1.0f / _text->common_block.scale_w;
	float y_height = 1.0f / _text->common_block.scale_h;

	for (uint16_t i = 0; i < _text->num_char_block; i++)
	{
		float x_tex = x_width * _text->char_block[i].x;
		float x_tex2 = x_width * (_text->char_block[i].x + _text->char_block[i].width);
		float y_tex = y_height * _text->char_block[i].y;
		float y_tex2 = y_height * (_text->char_block[i].y + _text->char_block[i].height);

		float x_pos = 0.0f;
		float x_pos2 = 1.0f;
		float y_pos = 0.0f;
		float y_pos2 = 1.0f;

		GLfloat vertices[] = {
			// Pos      // Tex
			x_pos, y_pos2, x_tex, y_tex2,
			x_pos2, y_pos, x_tex2, y_tex,
			x_pos, y_pos, x_tex, y_tex, 
    
			x_pos, y_pos2, x_tex, y_tex2,
			x_pos2, y_pos2, x_tex2, y_tex2,
			x_pos2, y_pos, x_tex2, y_tex
		};

		/*GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 1.0f, x_tex, y_tex2,
			1.0f, 0.0f, x_tex2, y_tex,
			0.0f, 0.0f, x_tex, y_tex, 
    
			0.0f, 1.0f, x_tex, y_tex2,
			1.0f, 1.0f, x_tex2, y_tex2,
			1.0f, 0.0f, x_tex2, y_tex
		};*/

		glGenVertexArrays(1, &_text->vao[i]);
		glBindVertexArray(_text->vao[i]);

		glGenBuffers(1, &_text->vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _text->vbo[i]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void text_load_fnt(text* _text, const char* _name)
{
	std::string name;
	name += _name;
	name += ".fnt";

	FILE* file;
	file = fopen(name.c_str(), "rb");

	if (file == NULL)
		log_fprint("ERROR: %s doesn't exist", name.c_str());

	else
	{
		char file_header[3];
		fread(file_header, 3, 1, file);
	
		//if the header of the file has the correct signiture
		if (file_header[0] == 'B' && file_header[1] == 'M'
			&& file_header[2] == 'F')
		{
			//start at the 4th byte (v3)
			fseek(file, 1, SEEK_CUR);

			//read in the block id
			uint8_t block_type_id;
			fread(&block_type_id, 1, 1, file);

			//get the size of this block
			uint32_t size_this_block;
			fread(&size_this_block, 4, 1, file);
		
			//get the size of the font in bytes (minus the font name in bytes)
			fread(&_text->info_block, sizeof(FNT_INFO_BLOCK), 1, file);

			//get the font name
			int16_t name_size = size_this_block - sizeof(FNT_INFO_BLOCK);
			_text->filename = (char*)malloc(name_size);
			fread(_text->filename, name_size, 1, file);

			//Each block starts with a one byte block type identifier, 
			//followed by a 4 byte integer that gives the size of the block
			fread(&block_type_id, 1, 1, file);
			fread(&size_this_block, 4, 1, file);

			fread(&_text->common_block, sizeof(FNT_COMMON_BLOCK), 1, file);

			fseek(file, -1, SEEK_CUR);

			//Each block starts with a one byte block type identifier, 
			//followed by a 4 byte integer that gives the size of the block
			fread(&block_type_id, 1, 1, file);
			fread(&size_this_block, 4, 1, file);

			_text->page_names = (char*)malloc(size_this_block);
			fread(_text->page_names, size_this_block, 1, file);

			//Each block starts with a one byte block type identifier, 
			//followed by a 4 byte integer that gives the size of the block
			fread(&block_type_id, 1, 1, file);
			fread(&size_this_block, 4, 1, file);
		
			_text->num_char_block = size_this_block / sizeof(FNT_CHAR_BLOCK);
			_text->char_block = (FNT_CHAR_BLOCK*)malloc(_text->num_char_block * sizeof(FNT_CHAR_BLOCK));
			fread(&_text->char_block[0], size_this_block, 1, file);

			//Each block starts with a one byte block type identifier, 
			//followed by a 4 byte integer that gives the size of the block
			fread(&block_type_id, 1, 1, file);
			fread(&size_this_block, 4, 1, file);

			fread(&_text->kerning_pair_block, sizeof(FNT_KERNING_PAIR_BLOCK), 1, file);
		}

		else
			log_fprint("Font's '.FNT' was not found!");
	}

	//shut that file down boy
	fclose(file);
}

void text_set_colour(text* _text, vec3 _colour)
{
	_text->font_colour = _colour;
}

//improve drawing by binding all textures onto 1 big quad (1 draw call)
//position coords become uv coords and the quad is the size of all letters combined.
void text_draw(std::string _str, text* _text, vec2 _pos)
{
	//spacing between each char in the text for the shader
	float char_index = 0.0f;

	_text->transform.position = _pos;

	//bind our program
	glUseProgram(_text->shader->program);
	
	//communicate w/ uniforms
	//send the model matrix off
	transform_get_model_matrix(_text->transform);
	shader_set_uniform_mat4(_text->model, _text->transform.model_matrix, true);

	//send the projection matrix off
	shader_set_uniform_mat4(_text->projection, _text->default_proj, false);

	//set the font colour
	shader_set_uniform_vec3(_text->colour, _text->font_colour);

	//bind our font texture
	texture2d_bind(_text->texture);

	//debug the first character '@'
	for (uint8_t i = 0; i < _str.size(); i++)
	{
		shader_set_uniform_float(_text->char_index, char_index);
		char_index += 1.0f;

		uint8_t index = (uint8_t)_str[i] - 32;
		glBindVertexArray(_text->vao[index]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}

void text_printf(text* _text, vec2 _pos, uint16_t _text_bytes, const char *fmt, ...)
{
	//spacing between each char in the text for the shader
	float char_index = 0.0f;

	_text->transform.position = _pos;

	//bind our program
	glUseProgram(_text->shader->program);
	
	//communicate w/ uniforms
	//send the model matrix off
	transform_get_model_matrix(_text->transform);
	shader_set_uniform_mat4(_text->model, _text->transform.model_matrix, true);

	//send the projection matrix off
	shader_set_uniform_mat4(_text->projection, _text->default_proj, false);

	//set the font colour
	shader_set_uniform_vec3(_text->colour, _text->font_colour);

	//bind our font texture
	texture2d_bind(_text->texture);

	char text[50];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	//debug the first character '@'
	for (uint8_t i = 0; i < _text_bytes; i++)
	{
		shader_set_uniform_float(_text->char_index, char_index);
		char_index += 1.0f;

		uint8_t index = (uint8_t)text[i] - 32;
		glBindVertexArray(_text->vao[index]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}

void text_destroy(text* _text)
{
	shader_destroy(_text->shader);
	texture2d_destroy(_text->texture);
	glDeleteBuffers(_text->num_char_block, &_text->vbo[0]);
	glDeleteBuffers(_text->num_char_block, &_text->vao[0]);
}