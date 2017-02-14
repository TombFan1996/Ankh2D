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

bool sprite_update(sprite* _sprite)
{
	SDL_PumpEvents();
	vec2 pos = vec2_create(0.0f, 0.0f);
	bool sprite_update = false;

	if (_sprite->keys[SDL_SCANCODE_W])
	{
		pos = vec2_create(pos.x + 0.0f, pos.y + -_sprite->speed);
		sprite_update = true;
	}

	if (_sprite->keys[SDL_SCANCODE_A])
	{
		pos = vec2_create(pos.x + -_sprite->speed, pos.y + 0.0f);
		sprite_update = true;
	}

	if (_sprite->keys[SDL_SCANCODE_S])
	{
		pos = vec2_create(pos.x + 0.0f, pos.y + _sprite->speed);
		sprite_update = true;
	}

	if (_sprite->keys[SDL_SCANCODE_D])
	{
		pos = vec2_create(pos.x + _sprite->speed, pos.y + 0.0f);
		sprite_update = true;
	}

	//only update if we move
	if (sprite_update)
	{
		_sprite->transform.position = vec2_create(_sprite->transform.position.x + pos.x, 
			_sprite->transform.position.y + pos.y);
	}

	return sprite_update;
}

void sprite_map_intersect(tmx_sprite* _tmx_map, sprite* _sprite, bool _sprite_update)
{
	//dont waste cycles if we havent moved the sprite
	if (_sprite_update)
	{
		if (_tmx_map->map->num_collisions != 0)
		{
			uint8_t current_x = 0, current_y = 0;
			for (uint8_t i = 0; i < (_tmx_map->map->map_height * _tmx_map->map->map_width); i++)
			{
				//if this tile has a collision on it
				if (_tmx_map->map->collision_data[i] == 1)
				{
					// - (_tmx_map->transform.scale.x / 2) is equal to how far we're into a tile in the coords 
					// if 1 tile is 50 px wide and the map is 5 tiles wide and you place it at vec2(0,0), this would mean
					// you're 25 pixels into the middle tile. (This is a prob, need to work it out realtime based off position)
					float x1 = (current_x * _tmx_map->transform.scale.x) - (_tmx_map->transform.scale.x / 2);
					if (_sprite->transform.position.x >= x1)
					{
						float x2 = ((current_x * _tmx_map->transform.scale.x) + _tmx_map->transform.scale.x) - (_tmx_map->transform.scale.x / 2);
						if (_sprite->transform.position.x <= x2)
						{
							float y1 = (current_y * _tmx_map->transform.scale.y) - (_tmx_map->transform.scale.y / 2);
							if (_sprite->transform.position.y >= y1)
							{
								float y2 = ((current_y * _tmx_map->transform.scale.y) + _tmx_map->transform.scale.y) - (_tmx_map->transform.scale.y / 2);
								if (_sprite->transform.position.y <= y2)
								{
									//revert back the pre-collision position
									_sprite->transform.position = _sprite->old_pos;

									break;
								}
							}
						}
					}				
				}

				current_x++;
				if (current_x == _tmx_map->map->map_width)
				{
					current_x = 0;
					current_y++;
				}
			}
		}
	}

	//update the old position (used for collisions)
	_sprite->old_pos = _sprite->transform.position;
}

void sprite_draw(sprite* _sprite, mat4* _projection)
{
	//bind our program
	glUseProgram(_sprite->shader->program);

	transform_get_model_matrix(_sprite->transform);
	shader_set_uniform_mat4(_sprite->model, _sprite->transform.model_matrix, true);
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
