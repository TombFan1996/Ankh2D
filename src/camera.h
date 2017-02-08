#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"
#include "transform.h"
#include <SDL.h>

#include "sprite.h"

typedef struct 
{
	bool free_camera;
	transform transform;
	const uint8_t* keys;
	float speed; //camera speed when free (not attached to player)
	float screen_width, screen_height;
	mat4 projection;
} camera;

camera camera_create(float _speed);
camera camera_create();
void camera_update(camera* _camera);
void camera_update(camera* _camera, sprite* _sprite);
void camera_movement(camera* _camera);

#endif