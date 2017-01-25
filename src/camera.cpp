#include "camera.h"

camera* mainCamera = new camera;

void camera_create()
{
	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	mainCamera->projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	mainCamera->transform = new transform;
	mainCamera->transform->position = glm::vec2(0.0f, 0.0f);
	mainCamera->transform->rotation = 0.0f;
	mainCamera->transform->scale = glm::vec2(40.0f, 40.0f);
	
	mainCamera->screenHeight = height;
	mainCamera->screenWidth = width;

	mainCamera->speed = 0.25f;
	mainCamera->camState = CDir::CAM_STILL;
	mainCamera->keys = SDL_GetKeyboardState(NULL);
	mainCamera->freeCamera = true;
}

camera* camera_create(float _speed)
{
	camera* newCamera = new camera;

	int width, height;
	SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
	mainCamera->projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	
	mainCamera->screenHeight = height;
	mainCamera->screenWidth = width;

	mainCamera->speed = _speed;
	mainCamera->camState = CDir::CAM_STILL;
	mainCamera->keys = SDL_GetKeyboardState(NULL);
	mainCamera->freeCamera = true;
	return newCamera;
}

void camera_update()
{
	camera_movement(mainCamera);

	//update orthographic camera normally
	glm::vec2 position = mainCamera->transform->position;
	mainCamera->projection = glm::ortho(position.x, position.x + mainCamera->screenWidth, 
		position.y + mainCamera->screenHeight, position.y, -1.0f, 1.0f);
}

void camera_update(camera* _camera)
{
	camera_movement(_camera);

	//update orthographic camera normally
	glm::vec2 position = _camera->transform->position;
	_camera->projection = glm::ortho(position.x, position.x + _camera->screenWidth, 
		position.y + _camera->screenHeight, position.y, -1.0f, 1.0f);
}

void camera_update(sprite* _sprite)
{
	camera_movement(mainCamera);

	//update orthographic camera following the sprite
	glm::vec2 targetPos = _sprite->transform->position;
	glm::vec2 position = mainCamera->transform->position;

	mainCamera->transform->position = glm::vec2(targetPos.x - (mainCamera->screenWidth/2), targetPos.y - (mainCamera->screenHeight/2));
	
	mainCamera->projection = glm::ortho(position.x, position.x + mainCamera->screenWidth, 
		position.y + mainCamera->screenHeight, position.y, -1.0f, 1.0f);
}

void camera_update(camera* _camera, sprite* _sprite)
{
	camera_movement(_camera);

	//update orthographic camera following the sprite
	glm::vec2 targetPos = _sprite->transform->position;
	glm::vec2 position = _camera->transform->position;

	_camera->transform->position = glm::vec2(targetPos.x - (_camera->screenWidth/2), targetPos.y - (_camera->screenHeight/2));
	
	_camera->projection = glm::ortho(position.x, position.x + _camera->screenWidth, 
		position.y + _camera->screenHeight, position.y, -1.0f, 1.0f);
}

void camera_movement(camera* _camera)
{
	SDL_PumpEvents();

	if (_camera->keys[SDL_SCANCODE_W])
		_camera->camState = CDir::CAM_UP;

	else if (_camera->keys[SDL_SCANCODE_A])
		_camera->camState = CDir::CAM_LEFT;

	else if (_camera->keys[SDL_SCANCODE_S])
		_camera->camState = CDir::CAM_DOWN;

	else if (_camera->keys[SDL_SCANCODE_D])
		_camera->camState = CDir::CAM_RIGHT;

	else
		_camera->camState = CDir::CAM_STILL;
	
	switch(_camera->camState)
	{
		case CDir::CAM_UP:
			_camera->transform->position = glm::vec2(_camera->transform->position.x, 
				_camera->transform->position.y - _camera->speed);
			break;
		case CDir::CAM_LEFT:
			_camera->transform->position = glm::vec2(_camera->transform->position.x - _camera->speed,  
				_camera->transform->position.y);
			break;
		case CDir::CAM_DOWN:
			_camera->transform->position = glm::vec2(_camera->transform->position.x, 
				_camera->transform->position.y + _camera->speed);
			break;
		case CDir::CAM_RIGHT:
			_camera->transform->position = glm::vec2(_camera->transform->position.x + _camera->speed, 
				_camera->transform->position.y);
			break;
	}
}

void camera_destroy()
{
	delete mainCamera;
}

void camera_destroy(camera* _camera)
{
	delete _camera;
}