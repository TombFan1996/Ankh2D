#include "sprite.h"

sprite* sprite_create(const char* _name, shader* _shader, transform _trans)
{
	sprite* new_sprite = (sprite*)malloc(sizeof(sprite));
	new_sprite->shader = _shader;
	new_sprite->texture = texture2d_create(_name);

	new_sprite->transform = _trans;
	
	new_sprite->model = shader_get_uniform_location(new_sprite->shader, "model");
	new_sprite->projection = shader_get_uniform_location(new_sprite->shader, "projection");

	//transparency on the font
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	new_sprite->speed = 0.10f;
	new_sprite->keys = SDL_GetKeyboardState(NULL);

	GLfloat vertices[] = {
		// Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
	};

	//quad is normalised for positional and tex coords
	glGenVertexArrays(1, &new_sprite->vao);
	glBindVertexArray(new_sprite->vao);
	//allocate two vertex buffers (vert + texcoord)
	glGenBuffers(1, &new_sprite->vbo);
    
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//GLint size = (how many float elements per vertex)

	//positional datas
	glBindBuffer(GL_ARRAY_BUFFER, new_sprite->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return new_sprite;
}

void sprite_update(sprite* _sprite)
{
	SDL_PumpEvents();
	vec2 pos = vec2_create(0.0f, 0.0f);

	if (_sprite->keys[SDL_SCANCODE_W])
		pos = vec2_create(pos.x + 0.0f, pos.y + -_sprite->speed);

	if (_sprite->keys[SDL_SCANCODE_A])
		pos = vec2_create(pos.x + -_sprite->speed, pos.y + 0.0f);

	if (_sprite->keys[SDL_SCANCODE_S])
		pos = vec2_create(pos.x + 0.0f, pos.y + _sprite->speed);

	if (_sprite->keys[SDL_SCANCODE_D])
		pos = vec2_create(pos.x + _sprite->speed, pos.y + 0.0f);

	_sprite->transform.position = vec2_create(_sprite->transform.position.x + pos.x, 
		_sprite->transform.position.y + pos.y);
}

bool sprite_check_map_collision(tmx_map* _tmx_map, vec2 _sprite_pos)
{
	//use the current map we're in, this makes it less intensive
	//if we use more maps than just one massive map
	uint8_t tile_size_x = _tmx_map->tile_width;
	uint8_t tile_size_y = _tmx_map->tile_height;
	uint8_t current_x = 0, current_y = 0;
	for (uint8_t i = 0; i < (_tmx_map->map_height * _tmx_map->map_width); i++)
	{
		//if this tile has a collision on it
		if (_tmx_map->collision_data[i] == 1)
		{
			//translate to tile space and get
			//all 4 pos of the quad tile
			vec2 tile_pos_min, tile_pos_max;
			tile_pos_min.x = current_x * tile_size_x;
			tile_pos_min.y = current_y * tile_size_y;

			tile_pos_max.x = tile_pos_min.x + tile_size_x;
			tile_pos_max.y = tile_pos_min.y + tile_size_y;

			if (_sprite_pos.x > tile_pos_min.x && _sprite_pos.x < tile_pos_max.x && 
					_sprite_pos.y > tile_pos_min.y && _sprite_pos.y < tile_pos_max.y)
				return true;
		}

		current_x++;
		if (current_x == _tmx_map->map_width){
			current_x = 0;
			current_y++;
		}
	}

	return false;
}

void sprite_draw(sprite* _sprite, mat4 _projection)
{
	//bind our program
	glUseProgram(_sprite->shader->program);

	//communicate w/ uniforms
	//send the model matrix off
	shader_set_uniform_mat4(_sprite->model, transform_get_model_matrix(_sprite->transform), true);

	//send the projection matrix off
	shader_set_uniform_mat4(_sprite->projection, _projection, false);

	//bind our texture
	texture2d_bind(_sprite->texture);

	//bind and draw our object
	glBindVertexArray(_sprite->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void sprite_set_texture(sprite* _sprite, texture2d* _tex)
{
	if (!_sprite->texture){
		free(_sprite->texture);
		_sprite->texture = NULL;
	}

	else
		_sprite->texture = _tex;
}

void sprite_destroy(sprite* _sprite)
{
	texture2d_destroy(_sprite->texture);

	shader_destroy(_sprite->shader);

	glDeleteBuffers(1, &_sprite->vao);
	glDeleteBuffers(1, &_sprite->vbo);
}