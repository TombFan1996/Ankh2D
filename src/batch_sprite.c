#include "batch_sprite.h"

#if ANKH2D_WIN32

	//draw id based on the index of the tile we want
	batch_sprite batch_sprite_create(int32_vec2 _pixel_coord, vec2 _tile_size, texture2d* _tex, shader* _shader, transform _trans)
	{
		batch_sprite new_batch_sprite;

		new_batch_sprite.model = shader_get_uniform_location(_shader, "model");
		new_batch_sprite.projection = shader_get_uniform_location(_shader, "projection");
	
		//transparency on the font
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		new_batch_sprite.transform = _trans;

		glGenVertexArrays(1, &new_batch_sprite.vao);
		glBindVertexArray(new_batch_sprite.vao);

		glGenBuffers(1, &new_batch_sprite.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, new_batch_sprite.vbo);

		float x = 1.0f / (_tex->width / _tile_size.x);
		float y = 1.0f / (_tex->height / _tile_size.y);

		float image_normal_x = x * _pixel_coord.x;
		float image_normal_y = y * _pixel_coord.y;

		//number of tiles * (24 (6 * 2 (xy) pos, 6 * 2 (xy) texcoords) * datatype
		uint16_t offset_bytes = 24 * sizeof(GLfloat);

		GLfloat vertices[] = {
			// Pos		// Tex
			0.0f, 1.0f, image_normal_x, image_normal_y + y,
			1.0f, 0.0f, image_normal_x + x, image_normal_y,
			0.0f, 0.0f, image_normal_x, image_normal_y, 
   
			0.0f, 1.0f, image_normal_x, image_normal_y + y,
			1.0f, 1.0f, image_normal_x + x, image_normal_y + y,
			1.0f, 0.0f, image_normal_x + x, image_normal_y
		};

		//fill the buffer w/ nothing, allocates it the correct size for glBufferSubData
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		//4 = pos x,y & tex_coord x,y
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);

		return new_batch_sprite;
	}

	void batch_sprite_draw(mat4* _projection, batch_sprite* _sprite)
	{
		//communicate w/ uniforms
		//send the model matrix off
		mat4 model_matrix = transform_get_model_matrix(_sprite->transform);
		shader_set_uniform_mat4(_sprite->model, &model_matrix, true);

		//send the projection matrix off
		shader_set_uniform_mat4(_sprite->projection, _projection, false);

		glBindVertexArray(_sprite->vao);
		//always 6 vertices between each face
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void batch_sprite_destroy(batch_sprite* _sprite)
	{
		glDeleteBuffers(1, &_sprite->vbo);
		glDeleteBuffers(1, &_sprite->vao);
	}

#endif