#include "tmx_sprite.h"

TMX_Sprite::TMX_Sprite(TMX_MAP _map, Shader* _shader, Transform* _trans)
{
	m_map = _map;
	m_shader = _shader;
	m_transform = _trans;

	m_model = m_shader->getUniformLocation("model");
	m_projection = m_shader->getUniformLocation("projection");

	//WARNING: havent factored in tilesets for now
	//WARNING: filename is going to be ruined.
	
	std::string filename("assets/");
	filename += m_map.tileset[0].filename;
	m_tilemap = new Texture2D(filename.c_str());
	
	m_vboSize = m_map.layer[0].data.size();

	uint16_t mapSizeX = m_map.layer[0].width;
	uint16_t mapSizeY = m_map.layer[0].height;

	uint8_t tileSizeX = m_map.tileset[0].tile_width;
	uint8_t tileSizeY = m_map.tileset[0].tile_height;

	//normalised coords for the whole texture
	//normalised texture size
	float image_normalX = (1.0f / m_tilemap->getWidth()) * tileSizeX;
	float image_normalY = (1.0f / m_tilemap->getHeight()) * tileSizeY;

	uint16_t numTilesX = m_tilemap->getWidth() / tileSizeX;
	uint16_t numTilesY = m_tilemap->getHeight() / tileSizeY;

	//create x amount of new vbos,vaos
	m_VAO = new GLuint[m_vboSize];
	m_VBO = new GLuint[m_vboSize];

	uint16_t currentTileX = 0;
	//starts on -1, 0 % tilesize = 0 
	//(currentTileY becomes 1 on the 1st frame)
	int16_t currentTileY = -1;

	for (uint32_t i = 0; i < m_vboSize; i++)
	{
		uint16_t index = m_map.layer[0].data[i].tile_id - 1;
		//get x and y coord via mod and div
		uint16_t tileCoordX = index % numTilesX;
		uint16_t tileCoordY = index / numTilesY;

		//what tile are we traversing
		currentTileX = i % mapSizeX;
		
		//if we are mod 0, new line of sprites
		if ((currentTileX % 4) == 0)
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

		glGenVertexArrays(1, &m_VAO[i]);
		glBindVertexArray(m_VAO[i]);

		//quad is normalised for positional and tex coords
		glGenBuffers(1, &m_VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
		//GL_STATIC_DRAW prevents 2d animations (look into GL_STREAM/GL_DYNAMIC)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//4 = pos x,y & tex_coord x,y
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);  
		glBindVertexArray(0);
	}
}

void TMX_Sprite::draw(glm::mat4 _projection)
{
	//bind our program
	glUseProgram(m_shader->getProgram());

	//communicate w/ uniforms
	//send the model matrix off
	m_shader->getUniformMat4(m_model, m_transform->getModelMatrix());

	//send the projection matrix off
	m_shader->getUniformMat4(m_projection, _projection);

	//bind our texture
	m_tilemap->bind();

	for (uint16_t i = 0; i < m_vboSize; i++)
	{
		glBindVertexArray(m_VAO[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
}

TMX_Sprite::~TMX_Sprite()
{
	delete m_tilemap;
	delete m_transform;

	for (uint16_t i = 0; i < m_vboSize; i++)
	{
		glDeleteBuffers(1, &m_VBO[i]);
		glDeleteBuffers(1, &m_VAO[i]);
	}
}
