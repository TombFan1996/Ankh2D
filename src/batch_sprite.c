#include "batch_sprite.h"

#if ANKH2D_WIN32

	batch_sprite batch_sprite_create(const char* _sprite_name, uint8_t _tilesize, shader* _shader, transform _trans)
	{
		batch_sprite new_batch_sprite;

		new_batch_sprite.shader = _shader;
		new_batch_sprite.tilemap = texture2d_create(_sprite_name);

		new_batch_sprite.model = shader_get_uniform_location(new_batch_sprite.shader, "model");
		new_batch_sprite.projection = shader_get_uniform_location(new_batch_sprite.shader, "projection");
	
		//transparency on the font
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		//normalised coords for the whole textures
		float image_normal_x = (1.0f / new_batch_sprite.tilemap.width) * _tilesize;
		float image_normal_y = (1.0f / new_batch_sprite.tilemap.height) * _tilesize;

		//number of tiles in the map
		uint16_t num_tiles_x = new_batch_sprite.tilemap.width / _tilesize;
		uint16_t num_tiles_y = new_batch_sprite.tilemap.height / _tilesize;

		//create x amount of transforms for each sprite based on how many sprites we have
		new_batch_sprite.transform = (transform*)malloc(sizeof(transform) * (num_tiles_x * num_tiles_y));
		//clone the positioning of the transforms to each sprite
		//no way we can ask for all sprite positions, can change via array index
		for (uint16_t i = 0; i < (num_tiles_x * num_tiles_y); i++){
			memcpy(&new_batch_sprite.transform[i], &_trans, sizeof(transform));
		}

		glGenVertexArrays(1, &new_batch_sprite.vao);
		glBindVertexArray(new_batch_sprite.vao);

		glGenBuffers(1, &new_batch_sprite.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, new_batch_sprite.vbo);

		//number of tiles * (24 (6 * 2 (xy) pos, 6 * 2 (xy) texcoords) * datatype
		uint16_t offset_bytes = 24 * sizeof(GLfloat);
		//fill the buffer w/ nothing, allocates it the correct size for glBufferSubData
		glBufferData(GL_ARRAY_BUFFER, offset_bytes * (num_tiles_x * num_tiles_y), NULL, GL_STATIC_DRAW);
	
		//loop through all the maps layers
		uint16_t tile_counter = 0;
		for (uint8_t y = 0; y < num_tiles_y; y++)
		{
			for (uint16_t x = 0; x < num_tiles_x; x++)
			{
				GLfloat vertices[] = {
					// Pos		// Tex
					0.0f, 1.0f, (image_normal_x * x), (image_normal_y * y) + image_normal_y,
					1.0f, 0.0f, (image_normal_x * x) + image_normal_x, (image_normal_y * y),
					0.0f, 0.0f, (image_normal_x * x), (image_normal_y * y), 
   
					0.0f, 1.0f, (image_normal_x * x), (image_normal_y * y) + image_normal_y,
					1.0f, 1.0f, (image_normal_x * x) + image_normal_x, (image_normal_y * y) + image_normal_y,
					1.0f, 0.0f, (image_normal_x * x) + image_normal_x, (image_normal_y * y)
				};

				//fill the buffer using an offset.
				glBufferSubData(GL_ARRAY_BUFFER, tile_counter * offset_bytes, sizeof(vertices), &vertices);
				//4 = pos x,y & tex_coord x,y
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);

				//iterate to the next sprite tile
				tile_counter++;
			}	
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0); 
		glBindVertexArray(0);

		log_fprint("'%s' successfully created", _sprite_name);

		return new_batch_sprite;
	}

	void batch_sprite_draw(uint8_t _draw_id, mat4* _projection, batch_sprite* _sprite)
	{
		//bind our program
		glUseProgram(_sprite->shader->program);

		//communicate w/ uniforms
		//send the model matrix off
		mat4 model_matrix = transform_get_model_matrix(_sprite->transform[_draw_id]);
		shader_set_uniform_mat4(_sprite->model, &model_matrix, true);

		//send the projection matrix off
		shader_set_uniform_mat4(_sprite->projection, _projection, false);

		//bind our texture
		texture2d_bind(&_sprite->tilemap);

		glBindVertexArray(_sprite->vao);
		//always 6 vertices between each face
		//glDrawArrays(GL_TRIANGLES, _draw_id * 6, (_draw_id * 6) + 6);
		glDrawArrays(GL_TRIANGLES, 0, (_draw_id * 6) + 6);
		glBindVertexArray(0);
	}

	void batch_sprite_destroy(batch_sprite* _sprite)
	{
		texture2d_destroy(&_sprite->tilemap);
		_sprite->shader = NULL;
		glDeleteBuffers(1, &_sprite->vbo);
		glDeleteBuffers(1, &_sprite->vao);
	}

#endif