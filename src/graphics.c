#include "graphics.h"

#if ANKH2D_WIN32
	graphics graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs)
	{
		//clear previous logs
		log_fclear();

		graphics main_graphics;

		main_graphics.title = _title;
		main_graphics.width = _width;
		main_graphics.height = _height;

		if (!glfwInit())
			log_fprint("Glfw failed to init");

		else
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

			glfwWindowHint(GLFW_RED_BITS, 8);
			glfwWindowHint(GLFW_GREEN_BITS, 8);
			glfwWindowHint(GLFW_BLUE_BITS, 8);
			glfwWindowHint(GLFW_ALPHA_BITS, 8);

			main_graphics.fullscreen = _fs;

			if (_fs)
				main_graphics.window = glfwCreateWindow(main_graphics.width, main_graphics.height, main_graphics.title, glfwGetPrimaryMonitor(), NULL);

			else
				main_graphics.window = glfwCreateWindow(main_graphics.width, main_graphics.height, main_graphics.title, NULL, NULL);

			if (!main_graphics.window)
				log_fprint("ERROR: Failed to create Glfw window");
		
			else
			{
				//create the GL context
				glfwMakeContextCurrent(main_graphics.window);
				//setup error callback
				glfwSetErrorCallback(graphics_error_callback);
				//setup keyboard callback
				glfwSetKeyCallback(main_graphics.window, graphics_input_callback);
				//always call after creating context
				//the wglSwapInterval extension won't be registered pre-context
				glfwSwapInterval(0);
			}
		}

		//get all GL functions for this OS/GPU
		GLenum status = glewInit();
		if (status != GLEW_OK)
			log_fprint("Glew failed to init");

		//identify the version of GL
		log_fprint((char*)glGetString(GL_VERSION));

		//identify the shading lang version
		log_fprint((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

		//identify the name of the renderer used
		log_fprint((char*)glGetString(GL_RENDERER));

		main_graphics.closed = false;

		main_graphics.num_frames = 0;
	
		//get the starting time
		main_graphics.start_time = glfwGetTime();

		log_fprint("glfw successfully created");

		return main_graphics;
	}

	void graphics_set_vsync(bool _vsync)
	{
		if (_vsync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void graphics_window_size_callback(GLFWwindow* window, int width, int height)
	{
		//update the main graphics width and height
	}

	void graphics_error_callback(int error, const char* description)
	{
		log_fprint("ERROR: %s", description);
	}

	void graphics_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void graphics_clear()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

#elif ANKH2D_PSX
	void graphics_create(graphics* _graphics, uint16_t _debug)
	{
		//init all the hardware
		ResetCallback();
		//Cold reset the gs
		ResetGraph(0);
		
		//init the 1st controller
		PadInit(0);
		
		//clear the vram
		graphics_clear_vram();

		//set debugging options
		//0 = off
		//1 = monitor
		//2 = dump
		SetGraphDebug(_debug);

		//set it to PAL if E, else NTSC
		if (*(char *)0xbfc7ff52=='E'){
			//log_fprint("PSX video mode: PAL");
			SetVideoMode(1); 
			SCREEN_WIDTH = 320;
			SCREEN_HEIGHT = 256;
		}

		else
		{
			//log_fprint("PSX video mode: NTSC");
			SetVideoMode(0);
			SCREEN_WIDTH = 320;
			SCREEN_HEIGHT = 240;
		}

		// set the graphics mode resolutions. You may also try using 'GsNONINTER' (read LIBOVR46.PDF in PSYQ/DOCS for detailed information)
		GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0); 
		// set the top left coordinates of the two buffers in video memory
		GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);

		// init the ordering tables
		// easier way to access GPU "packets"
		_graphics->ot[0].length = LENGTH_OT;
		_graphics->ot[1].length = LENGTH_OT;
		_graphics->ot[0].org = _graphics->ot_tag[0];
		_graphics->ot[1].org = _graphics->ot_tag[1];

		//setup pointers to actual ordering table array
		GsClearOt(0, 0, &_graphics->ot[0]);
		GsClearOt(0, 0, &_graphics->ot[1]);

		//log_fprint("Ordering table successfully setup");

		//should be and will be explicitly called, not really needed
		//but for hello world, we need
		graphics_setup_debug_font(5, 20, 0, 512);

		//set the background colour to black default
		_graphics->background_color = int8_vec3_create(0, 0, 0);
		_graphics->current_buffer = 0;
	}

	void graphics_setup_debug_font(uint8_t _pos_x, uint8_t _pos_y, uint8_t _bkg_clear, uint16_t _max_chars)
	{
		//loaded into VRAM/SGRAM
		//Basic fontpattern (4-bit, 256x128) 
		//initializes all the print streams.
		FntLoad(960, 256);
	
		//screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters
		//default: 5, 20, 320, 240, 0, 512
		SetDumpFnt(FntOpen(_pos_x, _pos_y, SCREEN_WIDTH, SCREEN_HEIGHT, _bkg_clear, _max_chars)); 
		//log_fprint("Loaded basic font pattern (4-bit, 256 x 128), and set attributes successfully");	
	}

	void graphics_set_background_color(graphics* _graphics, int8_vec3 _color)
	{
		_graphics->background_color.x = _color.x;
		_graphics->background_color.y = _color.y;
		_graphics->background_color.z = _color.z;
	}

	//clears the framebuffer (1024px X 512px) w/ colour black
	//ensures no garbage when loading new images
	void graphics_clear_vram()
	{
		RECT rectTL;
		setRECT(&rectTL, 0, 0, 1024, 512);
		ClearImage2(&rectTL, 0, 0, 0);
		// ensure that the VRAM is clear before exiting
		DrawSync(0); 
	}

	void graphics_pre_draw(graphics* _graphics)
	{
		//flush print buffer
		FntFlush(-1);
		//get the current buffer and setup "packets"
		//for rendering
		_graphics->current_buffer = GsGetActiveBuff();
		// setup the packet workbase
		GsSetWorkBase((PACKET*)_graphics->gpu_packet_area[_graphics->current_buffer]); 
		// clear the ordering table
		GsClearOt(0, 0, &_graphics->ot[_graphics->current_buffer]); 
	}
#endif

void graphics_update(graphics* _graphics)
{
	#if ANKH2D_WIN32
		glfwPollEvents();
		glfwSwapBuffers(_graphics->window);
	#elif ANKH2D_PSX
		//wait for all drawing
		DrawSync(0);
		//wait for v blank interrupt
		VSync(0);
		// flip the double buffers
		GsSwapDispBuff();
		// clear the ordering table with a background color.
		GsSortClear(_graphics->background_color.x, _graphics->background_color.y, 
			_graphics->background_color.z, &_graphics->ot[_graphics->current_buffer]);
		// Draw the ordering table for the CurrentBuffer
		GsDrawOt(&_graphics->ot[_graphics->current_buffer]);
	#endif
}

int graphics_check_quit(graphics* _graphics)
{
	#if ANKH2D_WIN32
		return !glfwWindowShouldClose(_graphics->window);
	#elif ANKH2D_PSX
		return 1; // infinite running for now..
	#endif
}

void graphics_get_fps(graphics* _graphics, double* _fps)
{
	#if ANKH2D_WIN32
		_graphics->end_time = glfwGetTime();
		double delta = _graphics->end_time - _graphics->start_time;
		_graphics->num_frames++;

		//limit the drawing of fps
		if (delta >= 1.0)
		{
			*_fps = (double)_graphics->num_frames / delta;
			//update our starting time again
			_graphics->start_time = _graphics->end_time;
			//reset the frames
			_graphics->num_frames = 0;
		}
	
	#elif ANKH2D_PSX

	#endif
}

void graphics_destroy(graphics* _graphics)
{
	#if ANKH2D_WIN32
		glfwDestroyWindow(_graphics->window);
		log_fprint("destroyed glfw window");
		glfwTerminate();
		log_fprint("destroyed glfw");
	#elif ANKH2D_PSX
	
	#endif
}