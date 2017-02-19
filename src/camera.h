#ifndef CAMERA_H
#define CAMERA_H

//#include <SDL.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "bmath.h"
#include "transform.h"
#include "sprite.h"

typedef struct 
{
	//mat4 projection;
	bool free_camera;
	transform transform;
	float speed; //camera speed when free (not attached to player)
	float screen_width, screen_height;
	GLFWwindow* window;
} camera;

camera camera_create(GLFWwindow* _window);
void camera_update(camera* _camera);
void camera_update(camera* _camera, sprite* _sprite);
bool camera_movement(camera* _camera);

#endif