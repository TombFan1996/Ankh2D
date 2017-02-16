#include "camera.h"

camera* camera_create(GLFWwindow* _window)
{
	camera* main_camera = (camera*)malloc(sizeof(camera));
	main_camera->window = _window;
	int width, height;
	glfwGetWindowSize(main_camera->window, &width, &height);

	main_camera->transform;
	main_camera->transform.position = vec2_create(0.0f, 0.0f);
	main_camera->transform.rotation = 0.0f;
	main_camera->transform.scale = vec2_create(40.0f, 40.0f);
	
	main_camera->screen_height = (float)height;
	main_camera->screen_width = (float)width;

	main_camera->speed = 0.25f;
	main_camera->free_camera = true;

	//SSE = __m128 (16 byte bound), float[4][4] also 16 bytes
	main_camera->projection = (mat4*)malloc(sizeof(mat4));
	mat4_orthographic(main_camera->projection, 0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	log_fprint("'main camera' successfully created");

	return main_camera;
}

void camera_update(camera* _camera)
{
	bool camera_updated = camera_movement(_camera);

	if (camera_updated)
	{
		//update orthographic camera normally
		vec2 position = _camera->transform.position;
		mat4_orthographic(_camera->projection, position.x, position.x + _camera->screen_width, 
			position.y + _camera->screen_height, position.y, -1.0f, 1.0f);
	}
}

void camera_update(camera* _camera, sprite* _sprite)
{
	bool camera_updated = camera_movement(_camera);

	//want the camera to snap to the sprite at the start
	//if (camera_updated)
	//{
		//update orthographic camera following the sprite
		vec2 targetPos = _sprite->transform.position;
		_camera->transform.position = vec2_create(targetPos.x - (_camera->screen_width/2), targetPos.y - (_camera->screen_height/2));
	
		vec2 position = _camera->transform.position;
		mat4_orthographic(_camera->projection, position.x, position.x + _camera->screen_width, 
			position.y + _camera->screen_height, position.y, -1.0f, 1.0f);
	//}
}

bool camera_movement(camera* _camera)
{
	vec2 pos = vec2_create(0.0f, 0.0f);
	bool sprite_update = false;
	
	if (glfwGetKey(_camera->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		pos = vec2_create(pos.x + 0.0f, pos.y + -_camera->speed);
		sprite_update = true;
	}

	if (glfwGetKey(_camera->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		pos = vec2_create(pos.x + -_camera->speed, pos.y + 0.0f);
		sprite_update = true;
	}

	if (glfwGetKey(_camera->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		pos = vec2_create(pos.x + 0.0f, pos.y + _camera->speed);
		sprite_update = true;
	}

	if (glfwGetKey(_camera->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		pos = vec2_create(pos.x + _camera->speed, pos.y + 0.0f);
		sprite_update = true;
	}

	if (sprite_update)
	{
		_camera->transform.position = vec2_create(_camera->transform.position.x + pos.x, 
			_camera->transform.position.y + pos.y);
	}

	return sprite_update;
}

void camera_destroy(camera* _camera)
{
	free(_camera->projection);
}