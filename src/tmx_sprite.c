#include "tmx_sprite.h"

tmx_sprite* tmx_sprite_create(const char* _mapName, shader* _shader, transform _trans)
{
	tmx_sprite* newMapSprite = (tmx_sprite*)malloc(sizeof(tmx_sprite));

	std::string mapName("assets/");
	mapName += _mapName;
	newMapSprite->map = tmx_parser_create(mapName.c_str());
	newMapSprite->shader = _shader;
	newMapSprite->transform = _trans;

	newMapSprite->model = shader_getUniformLocation(newMapSprite->shader, "model");
	newMapSprite->projection = shader_getUniformLocation(newMapSprite->shader, "projection");

	//WARNING: havent factored in tilesets for now
	//WARNING: filename is going to be ruined.
	
	std::string filename("assets/");
	filename += newMapSprite->map->tileset.filename;
	newMapSprite->tilemap = texture2d_create(filename.c_str());
	
	newMapSprite->numTiles = newMapSprite->map->layer.width * 
		newMapSprite->map->layer.height;

	uint16_t mapSizeX = newMapSprite->map->layer.width;
	uint16_t mapSizeY = newMapSprite->map->layer.height;

	uint8_t tileSizeX = newMapSprite->map->tileset.tile_width;
	uint8_t tileSizeY = newMapSprite->map->tileset.tile_height;

	//normalised coords for the whole textures
	float image_normalX = (1.0f / newMapSprite->tilemap->width) * tileSizeX;
	float image_normalY = (1.0f / newMapSprite->tilemap->height) * tileSizeY;

	//number of tiles in the map
	uint16_t numTilesX = newMapSprite->tilemap->width / tileSizeX;
	uint16_t numTilesY = newMapSprite->tilemap->height / tileSizeY;
	
	uint16_t currentTileX = 0;
	//starts on -1, 0 % tilesize = 0 
	//(currentTileY becomes 1 on the 1st frame)
	int16_t currentTileY = -1;

	glGenVertexArrays(1, &newMapSprite->vao);
	glBindVertexArray(newMapSprite->vao);

	glGenBuffers(1, &newMapSprite->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newMapSprite->vbo);

	//number of tiles * (24 (6 * 2 (xy) pos, 6 * 2 (xy) texcoords) * datatype
	uint16_t offsetBytes = 24 * sizeof(GLfloat);
	uint32_t totalBytes = newMapSprite->numTiles * offsetBytes;
	//fill the buffer w/ nothing, allocates it the correct size for glBufferSubData
	glBufferData(GL_ARRAY_BUFFER, totalBytes, NULL, GL_STATIC_DRAW);

	for (uint32_t i = 0; i < newMapSprite->numTiles; i++)
	{
		uint16_t index = newMapSprite->map->layer.data[i].tile_id - 1;

		//what tile are we traversing
		currentTileX = i % mapSizeX;
		
		//if we are mod 0, new line of sprites
		if ((currentTileX % mapSizeX) == 0)
			currentTileY++;

		//get x and y coord via mod and div
		uint16_t tileCoordX = index % numTilesX;
		uint16_t tileCoordY = index / numTilesY;

		GLfloat vertices[] = {
			// Pos									  // Tex
			0.0f + currentTileX, 1.0f + currentTileY, (image_normalX * tileCoordX), (image_normalY * tileCoordY) + image_normalY,
			1.0f + currentTileX, 0.0f + currentTileY, (image_normalX * tileCoordX) + image_normalX, (image_normalY * tileCoordY),
			0.0f + currentTileX, 0.0f + currentTileY, (image_normalX * tileCoordX), (image_normalY * tileCoordY), 
   
			0.0f + currentTileX, 1.0f + currentTileY, (image_normalX * tileCoordX), (image_normalY * tileCoordY) + image_normalY,
			1.0f + currentTileX, 1.0f + currentTileY, (image_normalX * tileCoordX) + image_normalX, (image_normalY * tileCoordY) + image_normalY,
			1.0f + currentTileX, 0.0f + currentTileY, (image_normalX * tileCoordX) + image_normalX, (image_normalY * tileCoordY)
		};

		//fill the buffer using an offset.
		glBufferSubData(GL_ARRAY_BUFFER, i * offsetBytes, sizeof(vertices), &vertices);
		//4 = pos x,y & tex_coord x,y
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	}	

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	return newMapSprite;
}

void tmx_sprite_draw(tmx_sprite* _sprite, mat4 _projection)
{
	//bind our program
	glUseProgram(_sprite->shader->program);

	//communicate w/ uniforms
	//send the model matrix off
	shader_setUniformMat4(_sprite->model, transform_getModelMatrix(_sprite->transform), false);

	//send the projection matrix off
	shader_setUniformMat4(_sprite->projection, _projection, false);

	//bind our texture
	texture2d_bind(_sprite->tilemap);

	glBindVertexArray(_sprite->vao);
	glDrawArrays(GL_TRIANGLES, 0, 6 * _sprite->numTiles);
	glBindVertexArray(0);
}

void tmx_sprite_destroy(tmx_sprite* _sprite)
{
	tmx_parser_destroy(_sprite->map);
	texture2d_destroy(_sprite->tilemap);

	glDeleteBuffers(1, &_sprite->vbo);
	glDeleteBuffers(1, &_sprite->vao);
}
