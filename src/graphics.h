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
	uint32_t startTime, endTime;
	uint32_t numFrames;
	uint16_t width, height;
	SDL_Window* window;
	SDL_GLContext glContext;
} graphics;

extern graphics* mainGraphics;

graphics* graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs);
void graphics_clear();
void graphics_update();
void graphics_getFPS();
void graphics_destroy();

#endif