#ifndef CAMERA_H
#define CAMERA_H

#include "math.h"
#include "transform.h"
#include <SDL.h>

#include "sprite.h"

typedef struct 
{
	bool freeCamera;
	transform transform;
	const uint8_t* keys;
	float speed; //camera speed when free (not attached to player)
	float screenWidth, screenHeight;
	mat4 projection;
} camera;

extern camera* mainCamera;

camera* camera_create(float _speed);
void camera_create();
void camera_update(camera* _camera);
void camera_update(camera* _camera, sprite* _sprite);
void camera_movement(camera* _camera);
void camera_destroy();
void camera_destroy(camera* _camera);

#endif