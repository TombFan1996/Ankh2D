#include "tmx_sprite.h"

tmx_sprite* tmx_sprite_create(const char* _mapName, shader* _shader, transform _trans)
{
	tmx_sprite* new_map_sprite = (tmx_sprite*)malloc(sizeof(tmx_sprite));
	new_map_sprite->map = tmx_parser_create(_mapName);
	new_map_sprite->shader = _shader;

	new_map_sprite->transform = _trans;
	//inverse the y scale as we now use BMP
	//newMapSprite->transform.scale.y = 1.0f - newMapSprite->transform.scale.y;

	new_map_sprite->model = shader_get_uniform_location(new_map_sprite->shader, "model");
	new_map_sprite->projection = shader_get_uniform_location(new_map_sprite->shader, "projection");
	
	//transparency on the font
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	std::string filename("assets/");
	filename += new_map_sprite->map->tileset[0].filename;
	new_map_sprite->tilemap = texture2d_create(filename.c_str());
	
	//get the number of tiles per layer
	new_map_sprite->num_tiles = new_map_sprite->map->layer[0].width * new_map_sprite->map->layer[0].height;
	//get how many layers we have
	new_map_sprite->num_layers = new_map_sprite->map->num_layers;

	//get the width and height of the map 
	//(doesn't matter which layer, all layers will be the same)
	uint16_t map_size_x = new_map_sprite->map->layer[0].width;
	uint16_t map_size_y = new_map_sprite->map->layer[0].height;

	//get the size of each tile in px
	uint16_t tile_size_x = new_map_sprite->map->tileset[0].tile_width;
	uint16_t tile_size_y = new_map_sprite->map->tileset[0].tile_height;

	//normalised coords for the whole textures
	float image_normal_x = (1.0f / new_map_sprite->tilemap->width) * tile_size_x;
	float image_normal_y = (1.0f / new_map_sprite->tilemap->height) * tile_size_y;

	//number of tiles in the map
	uint16_t num_tiles_x = new_map_sprite->tilemap->width / tile_size_x;
	uint16_t num_tiles_y = new_map_sprite->tilemap->height / tile_size_y;

	glGenVertexArrays(1, &new_map_sprite->vao);
	glBindVertexArray(new_map_sprite->vao);

	glGenBuffers(1, &new_map_sprite->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, new_map_sprite->vbo);

	//number of tiles * (24 (6 * 2 (xy) pos, 6 * 2 (xy) texcoords) * datatype
	uint16_t offset_bytes = 24 * sizeof(GLfloat);
	uint32_t total_bytes = (new_map_sprite->num_layers * new_map_sprite->num_tiles) * offset_bytes;
	//fill the buffer w/ nothing, allocates it the correct size for glBufferSubData
	glBufferData(GL_ARRAY_BUFFER, total_bytes, NULL, GL_STATIC_DRAW);
	
	//loop through all the maps layers
	uint16_t tile_counter = 0;
	for (uint8_t layer = 0; layer < new_map_sprite->num_layers; layer++)
	{
		uint16_t current_tile_x = 0;
		//starts on -1, 0 % tilesize = 0 
		//(current_tile_y becomes 1 on the 1st frame)
		int16_t current_tile_y = -1;

		for (uint16_t tile = 0; tile < new_map_sprite->num_tiles; tile++)
		{
			//cannot do -1 on unsigned int, underflow will occur, we do
			//the -1 later when we know the tile isn't 0
			uint16_t index = new_map_sprite->map->layer[layer].data[tile];

			//what tile are we traversing
			current_tile_x = tile % map_size_x;
		
			//if we are mod 0, new line of sprites
			if ((current_tile_x % map_size_x) == 0)
				current_tile_y++;

			//no point creating a tile if it hasnt been assigned a tile id
			if (index != 0)
			{
				uint16_t tileCoordX = (index - 1) % num_tiles_x;
				uint16_t tileCoordY = (index - 1) / num_tiles_y;

				GLfloat vertices[] = {
					// Pos									  // Tex
					0.0f + current_tile_x, 1.0f + current_tile_y, (image_normal_x * tileCoordX), (image_normal_y * tileCoordY) + image_normal_y,
					1.0f + current_tile_x, 0.0f + current_tile_y, (image_normal_x * tileCoordX) + image_normal_x, (image_normal_y * tileCoordY),
					0.0f + current_tile_x, 0.0f + current_tile_y, (image_normal_x * tileCoordX), (image_normal_y * tileCoordY), 
   
					0.0f + current_tile_x, 1.0f + current_tile_y, (image_normal_x * tileCoordX), (image_normal_y * tileCoordY) + image_normal_y,
					1.0f + current_tile_x, 1.0f + current_tile_y, (image_normal_x * tileCoordX) + image_normal_x, (image_normal_y * tileCoordY) + image_normal_y,
					1.0f + current_tile_x, 0.0f + current_tile_y, (image_normal_x * tileCoordX) + image_normal_x, (image_normal_y * tileCoordY)
				};

				//fill the buffer using an offset.
				glBufferSubData(GL_ARRAY_BUFFER, tile_counter * offset_bytes, sizeof(vertices), &vertices);
				//4 = pos x,y & tex_coord x,y
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
				glEnableVertexAttribArray(0);
			}

			tile_counter++;
		}	
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	log_fprint("'%s' successfully created", _mapName);

	return new_map_sprite;
}

void tmx_sprite_draw(mat4* _projection, tmx_sprite* _sprite)
{
	//bind our program
	glUseProgram(_sprite->shader->program);

	//communicate w/ uniforms
	//send the model matrix off
	transform_get_model_matrix(_sprite->transform);
	mat4 model_matrix = transform_get_model_matrix(_sprite->transform);
	shader_set_uniform_mat4(_sprite->model, &model_matrix, true);

	//send the projection matrix off
	shader_set_uniform_mat4(_sprite->projection, _projection, false);

	//bind our texture
	texture2d_bind(_sprite->tilemap);

	glBindVertexArray(_sprite->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6 * (_sprite->num_tiles * _sprite->num_layers));
	glBindVertexArray(0);
}

void tmx_sprite_destroy(tmx_sprite* _sprite)
{
	tmx_parser_destroy(_sprite->map);
	texture2d_destroy(_sprite->tilemap);

	glDeleteBuffers(1, &_sprite->vbo);
	glDeleteBuffers(1, &_sprite->vao);
}
