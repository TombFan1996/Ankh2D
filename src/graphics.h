#ifndef GRAPHICS_H
#define GRAPHICS_H

//things to note:
//PS1 has 1mb of VRAM

#if ANKH2D_WIN32
	#include <GL/glew.h>
	#include <GLFW\glfw3.h>
	#include <sstream>

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

#elif ANKH2D_PSX
	#include <stdlib.h>
	#include <libgte.h>
	#include <libgpu.h>
	#include <libgs.h>

	#include "types.h"
	#include "bmath.h"

	uint16_t SCREEN_WIDTH;
	uint16_t SCREEN_HEIGHT;

	#define LENGTH_OT 1
	#define MAX_OT 2
	#define MAX_SPRITE 18

	static uint64_t __ramsize   = 0x00200000; // force 2 megabytes of RAM
	static uint64_t __stacksize = 0x00004000; // force 16 kilobytes of stack

	typedef struct
	{
		//double buffering
		//note: If you use interlace mode with a height of 480 lines, 
		//it may not be possible or practical to set up a double buffer
		//DISPENV disp[2];
		//DRAWENV draw[2];
		
		int16_t current_buffer;
		GsOT ot[MAX_OT];
		GsOT_TAG ot_tag[MAX_OT][1<<LENGTH_OT];
		PACKET gpu_packet_area[2][MAX_SPRITE];
		int8_vec3 background_color;
	} graphics;
#endif

#include "logger.h"

//generic all purpose functions (for most platforms)
int graphics_check_quit(graphics* _graphics);
void graphics_update(graphics* _graphics);
void graphics_get_fps(graphics* _graphics, double* _fps);
void graphics_destroy(graphics* _graphics);

//platform specific

#if ANKH2D_WIN32
	graphics graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs);
	void graphics_set_vsync(bool _vsync);
	void graphics_clear();
	void graphics_error_callback(int error, const char* description);
	void graphics_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void graphics_window_size_callback(GLFWwindow* window, int width, int height);
#elif ANKH2D_PSX
	void graphics_create(graphics* _graphics, uint16_t _debug);
	void graphics_pre_draw(graphics* _graphics);
	void graphics_set_background_color(graphics* _graphics, int8_vec3 _color);
	void graphics_setup_debug_font(uint8_t _pos_x, uint8_t _pos_y, uint8_t _bkg_clear, uint16_t _max_chars);
	void graphics_clear_vram();
#endif

#endif