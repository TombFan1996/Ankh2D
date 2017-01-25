#include "sprite.h"

sprite* sprite_create(const char* _name, shader* _shader, transform* _trans)
{
	sprite* newSprite = new sprite;
	newSprite->shader = _shader;
	newSprite->texture = texture2d_create(_name);
	newSprite->transform = _trans;

	newSprite->model = shader_getUniformLocation(newSprite->shader, "model");
	newSprite->projection = shader_getUniformLocation(newSprite->shader, "projection");

	newSprite->speed = 0.25f;
	newSprite->spriteState = SDir::SPRITE_STILL;
	newSprite->keys = SDL_GetKeyboardState(NULL);

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
	glGenVertexArrays(1, &newSprite->vao);
	glBindVertexArray(newSprite->vao);
	//allocate two vertex buffers (vert + texcoord)
	glGenBuffers(1, &newSprite->vbo);
    
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//GLint size = (how many float elements per vertex)

	//positional datas
	glBindBuffer(GL_ARRAY_BUFFER, newSprite->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return newSprite;
}

void sprite_update(sprite* _sprite)
{
	SDL_PumpEvents();

	if (_sprite->keys[SDL_SCANCODE_W])
		_sprite->spriteState = SDir::SPRITE_UP;

	else if (_sprite->keys[SDL_SCANCODE_A])
		_sprite->spriteState = SDir::SPRITE_LEFT;

	else if (_sprite->keys[SDL_SCANCODE_S])
		_sprite->spriteState = SDir::SPRITE_DOWN;

	else if (_sprite->keys[SDL_SCANCODE_D])
		_sprite->spriteState = SDir::SPRITE_RIGHT;

	else
		_sprite->spriteState = SDir::SPRITE_STILL;
	
	switch(_sprite->spriteState)
	{
		case SDir::SPRITE_UP:
			_sprite->transform->position = glm::vec2(_sprite->transform->position.x, 
				_sprite->transform->position.y - _sprite->speed);
			break;
		case SDir::SPRITE_LEFT:
			_sprite->transform->position = glm::vec2(_sprite->transform->position.x - _sprite->speed, 
				_sprite->transform->position.y);
			break;
		case SDir::SPRITE_DOWN:
			_sprite->transform->position = glm::vec2(_sprite->transform->position.x, 
				_sprite->transform->position.y + _sprite->speed);
			break;
		case SDir::SPRITE_RIGHT:
			_sprite->transform->position = glm::vec2(_sprite->transform->position.x + _sprite->speed, 
				_sprite->transform->position.y);
			break;
	}
}

void sprite_draw(sprite* _sprite, glm::mat4 _projection)
{
	//bind our program
	glUseProgram(_sprite->shader->program);

	//communicate w/ uniforms
	//send the model matrix off
	shader_setUniformMat4(_sprite->model, transform_getModelMatrix(_sprite->transform));

	//send the projection matrix off
	shader_setUniformMat4(_sprite->projection, _projection);

	//bind our texture
	texture2d_bind(_sprite->texture);

	//bind and draw our object
	glBindVertexArray(_sprite->vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void sprite_setTexture(sprite* _sprite, texture2d* _tex)
{
	if (!_sprite->texture)
		delete _sprite->texture;
	else
		_sprite->texture = _tex;
}

void sprite_destroy(sprite* _sprite)
{
	texture2d_destroy(_sprite->texture);
	//delete _sprite->transform;

	shader_destroy(_sprite->shader);

	glDeleteBuffers(1, &_sprite->vao);
	glDeleteBuffers(1, &_sprite->vbo);
}