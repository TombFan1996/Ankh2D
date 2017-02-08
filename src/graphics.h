#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <GL/glew.h>
#include <stdint.h>
#include <sstream>

#include "logger.h"

typedef struct
{
	bool closed;
	const char* title;
	uint32_t start_time, end_time;
	uint32_t num_frames;
	uint16_t width, height;
	SDL_Window* window;
	SDL_GLContext gl_context;
} graphics;

graphics graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs);
void graphics_clear();
void graphics_update(graphics* _graphics);
void graphics_get_fps(graphics* _graphics);
void graphics_destroy(graphics* _graphics);

#endif