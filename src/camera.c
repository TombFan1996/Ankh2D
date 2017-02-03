#include "camera.h"

camera* mainCamera = (camera*)malloc(sizeof(camera));

void camera_create()
{
	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	mainCamera->projection = mat4_getOrthographic(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	mainCamera->transform;
	mainCamera->transform.position = vec2_create(0.0f, 0.0f);
	mainCamera->transform.rotation = 0.0f;
	mainCamera->transform.scale = vec2_create(40.0f, 40.0f);
	
	mainCamera->screenHeight = height;
	mainCamera->screenWidth = width;

	mainCamera->speed = 0.25f;
	mainCamera->keys = SDL_GetKeyboardState(NULL);
	mainCamera->freeCamera = true;
}

camera* camera_create(float _speed)
{
	camera* newCamera = (camera*)malloc(sizeof(camera));

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	mainCamera->projection = mat4_getOrthographic(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	
	mainCamera->screenHeight = height;
	mainCamera->screenWidth = width;

	mainCamera->speed = _speed;
	mainCamera->keys = SDL_GetKeyboardState(NULL);
	mainCamera->freeCamera = true;
	return newCamera;
}

void camera_update(camera* _camera)
{
	camera_movement(_camera);

	//update orthographic camera normally
	vec2 position = _camera->transform.position;
	_camera->projection = mat4_getOrthographic(position.x, position.x + _camera->screenWidth, 
		position.y + _camera->screenHeight, position.y, -1.0f, 1.0f);
}

void camera_update(camera* _camera, sprite* _sprite)
{
	camera_movement(_camera);

	//update orthographic camera following the sprite
	vec2 targetPos = _sprite->transform.position;
	_camera->transform.position = vec2_create(targetPos.x - (_camera->screenWidth/2), targetPos.y - (_camera->screenHeight/2));
	
	vec2 position = _camera->transform.position;
	_camera->projection = mat4_getOrthographic(position.x, position.x + _camera->screenWidth, 
		position.y + _camera->screenHeight, position.y, -1.0f, 1.0f);
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

void camera_destroy()
{
	delete mainCamera;
}

void camera_destroy(camera* _camera)
{
	delete _camera;
}