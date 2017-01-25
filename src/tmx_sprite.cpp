#include "tmx_sprite.h"

tmx_sprite* tmx_sprite_create(tmx_map* _map, shader* _shader, transform* _trans)
{
	tmx_sprite* newMapSprite = new tmx_sprite;
	newMapSprite->map = _map;
	newMapSprite->shader = _shader;
	newMapSprite->transform = _trans;

	newMapSprite->model = shader_getUniformLocation(newMapSprite->shader, "model");
	newMapSprite->projection = shader_getUniformLocation(newMapSprite->shader, "projection");

	//WARNING: havent factored in tilesets for now
	//WARNING: filename is going to be ruined.
	
	std::string filename("assets/");
	filename += newMapSprite->map->tileset[0].filename;
	newMapSprite->tilemap = texture2d_create(filename.c_str());
	
	newMapSprite->vboSize = newMapSprite->map->layer[0].data.size();

	uint16_t mapSizeX = newMapSprite->map->layer[0].width;
	uint16_t mapSizeY = newMapSprite->map->layer[0].height;

	uint8_t tileSizeX = newMapSprite->map->tileset[0].tile_width;
	uint8_t tileSizeY = newMapSprite->map->tileset[0].tile_height;

	//normalised coords for the whole textures
	float image_normalX = (1.0f / newMapSprite->tilemap->width) * tileSizeX;
	float image_normalY = (1.0f / newMapSprite->tilemap->height) * tileSizeY;

	//number of tiles in the map
	uint16_t numTilesX = newMapSprite->tilemap->width / tileSizeX;
	uint16_t numTilesY = newMapSprite->tilemap->height / tileSizeY;

	//create x amount of new vbos,vaos
	newMapSprite->vao = new GLuint[newMapSprite->vboSize];
	newMapSprite->vbo = new GLuint[newMapSprite->vboSize];
	
	uint16_t currentTileX = 0;
	//starts on -1, 0 % tilesize = 0 
	//(currentTileY becomes 1 on the 1st frame)
	int16_t currentTileY = -1;

	for (uint32_t i = 0; i < newMapSprite->vboSize; i++)
	{
		uint16_t index = newMapSprite->map->layer[0].data[i].tile_id - 1;
		//get x and y coord via mod and div
		uint16_t tileCoordX = index % numTilesX;
		uint16_t tileCoordY = index / numTilesY;

		//what tile are we traversing
		currentTileX = i % mapSizeX;
		
		//if we are mod 0, new line of sprites
		if ((currentTileX % numTilesX) == 0)
			currentTileY++;

		GLfloat vertices[] = {
			// Pos									  // Tex
			0.0f + currentTileX, 1.0f + currentTileY, (image_normalX * tileCoordX), (image_normalY * tileCoordY) + image_normalY,
			1.0f + currentTileX, 0.0f + currentTileY, (image_normalX * tileCoordX) + image_normalX, (image_normalY * tileCoordY),
			0.0f + currentTileX, 0.0f + currentTileY, (image_normalX * tileCoordX), (image_normalY * tileCoordY), 
   
			0.0f + currentTileX, 1.0f + currentTileY, (image_normalX * tileCoordX), (image_normalY * tileCoordY) + image_normalY,
			1.0f + currentTileX, 1.0f + currentTileY, (image_normalX * tileCoordX) + image_normalX, (image_normalY * tileCoordY) + image_normalY,
			1.0f + currentTileX, 0.0f + currentTileY, (image_normalX * tileCoordX) + image_normalX, (image_normalY * tileCoordY)
		};

		glGenVertexArrays(1, &newMapSprite->vao[i]);
		glBindVertexArray(newMapSprite->vao[i]);

		//quad is normalised for positional and tex coords
		glGenBuffers(1, &newMapSprite->vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, newMapSprite->vbo[i]);
		//GL_STATIC_DRAW prevents 2d animations (look into GL_STREAM/GL_DYNAMIC)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//4 = pos x,y & tex_coord x,y
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);  
		glBindVertexArray(0);
	}

	return newMapSprite;
}

void tmx_sprite_draw(tmx_sprite* _sprite, glm::mat4 _projection)
{
	//bind our program
	glUseProgram(_sprite->shader->program);

	//communicate w/ uniforms
	//send the model matrix off
	shader_setUniformMat4(_sprite->model, transform_getModelMatrix(_sprite->transform));

	//send the projection matrix off
	shader_setUniformMat4(_sprite->projection, _projection);

	//bind our texture
	texture2d_bind(_sprite->tilemap);

	for (uint16_t i = 0; i < _sprite->vboSize; i++)
	{
		glBindVertexArray(_sprite->vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}

void tmx_sprite_destroy(tmx_sprite* _sprite)
{
	texture2d_destroy(_sprite->tilemap);
	//delete _sprite->transform;

	glDeleteBuffers(_sprite->vboSize, _sprite->vbo);
	glDeleteBuffers(_sprite->vboSize, _sprite->vao);
	
	_sprite->map = nullptr;
}
