#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include <SDL.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <sstream>

#include "logger.h"

typedef struct
{
	bool closed;
	const char* title;
	double start_time, end_time;
	uint32_t num_frames;
	uint16_t width, height;
	GLFWwindow* window;
	bool fullscreen;
} graphics;

void graphics_error_callback(int error, const char* description);
void graphics_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void graphics_window_size_callback(GLFWwindow* window, int width, int height);
graphics* graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs);
void graphics_clear();
void graphics_set_swap_interval(bool _vsync);
void graphics_update(graphics* _graphics);
void graphics_get_fps(graphics* _graphics, double* _fps);
void graphics_destroy(graphics* _graphics);

#endif