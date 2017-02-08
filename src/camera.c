#include "camera.h"

camera camera_create()
{
	camera main_camera;

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	main_camera.projection = mat4_orthographic(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	main_camera.transform;
	main_camera.transform.position = vec2_create(0.0f, 0.0f);
	main_camera.transform.rotation = 0.0f;
	main_camera.transform.scale = vec2_create(40.0f, 40.0f);
	
	main_camera.screen_height = height;
	main_camera.screen_width = width;

	main_camera.speed = 0.25f;
	main_camera.keys = SDL_GetKeyboardState(NULL);
	main_camera.free_camera = true;

	return main_camera;
}

camera camera_create(float _speed)
{
	camera main_camera;

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	main_camera.projection = mat4_orthographic(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	
	main_camera.screen_height = height;
	main_camera.screen_width = width;

	main_camera.speed = _speed;
	main_camera.keys = SDL_GetKeyboardState(NULL);
	main_camera.free_camera = true;

	return main_camera;
}

void camera_update(camera* _camera)
{
	camera_movement(_camera);

	//update orthographic camera normally
	vec2 position = _camera->transform.position;
	_camera->projection = mat4_orthographic(position.x, position.x + _camera->screen_width, 
		position.y + _camera->screen_height, position.y, -1.0f, 1.0f);
}

void camera_update(camera* _camera, sprite* _sprite)
{
	camera_movement(_camera);

	//update orthographic camera following the sprite
	vec2 targetPos = _sprite->transform.position;
	_camera->transform.position = vec2_create(targetPos.x - (_camera->screen_width/2), targetPos.y - (_camera->screen_height/2));
	
	vec2 position = _camera->transform.position;
	_camera->projection = mat4_orthographic(position.x, position.x + _camera->screen_width, 
		position.y + _camera->screen_height, position.y, -1.0f, 1.0f);
}

void camera_movement(camera* _camera)
{
	vec2 pos = vec2_create(0.0f, 0.0f);

	if (_camera->keys[SDL_SCANCODE_W])
		pos = vec2_create(pos.x + 0.0f, pos.y + -_camera->speed);

	if (_camera->keys[SDL_SCANCODE_A])
		pos = vec2_create(pos.x + -_camera->speed, pos.y + 0.0f);

	if (_camera->keys[SDL_SCANCODE_S])
		pos = vec2_create(pos.x + 0.0f, pos.y + _camera->speed);

	if (_camera->keys[SDL_SCANCODE_D])
		pos = vec2_create(pos.x + _camera->speed, pos.y + 0.0f);

	_camera->transform.position = vec2_create(_camera->transform.position.x + pos.x, 
		_camera->transform.position.y + pos.y);
}