#include "graphics.h"

graphics* mainGraphics = new graphics;

graphics* graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs)
{
	//clear previous logs
	log_fclear();

	mainGraphics->title = _title;
	mainGraphics->width = _width;
	mainGraphics->height = _height;
	log_fprint("Window Size: %i x %i", _width, _height);

	//setup SDL+GL windows and contexts
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	log_fprint("OpenGL 3.1");

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	log_fprint("RGBA: 32 bits");

	//disable vsync
	SDL_GL_SetSwapInterval(0);
	log_fprint("VSync: disabled");

	//SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	//log_fprint("Buffer size: 32 bits");
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//log_fprint("Double buffering is enabled");

	if (_fs)
	{
		mainGraphics->window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			mainGraphics->width, mainGraphics->height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
		log_fprint("Fullscreen mode");
	}

	else
	{
		mainGraphics->window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			mainGraphics->width, mainGraphics->height, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
		log_fprint("Windowed mode");
	}

	mainGraphics->glContext = SDL_GL_CreateContext(mainGraphics->window);

	//get all GL functions for this OS/GPU
	GLenum status = glewInit();
	if (status != GLEW_OK)
		log_fprint("Glew failed to init");

	mainGraphics->closed = false;

	mainGraphics->numFrames = 0;
	//get the starting time
	mainGraphics->startTime = SDL_GetTicks();

	return mainGraphics;
}

void graphics_clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void graphics_update()
{
	SDL_GL_SwapWindow(mainGraphics->window);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			mainGraphics->closed = true;

		if (event.key.keysym.sym == SDLK_ESCAPE)
			mainGraphics->closed = true;
	}

	#ifdef ANKH2D_DEBUG
		graphics_getFPS();
	#endif
}

void graphics_getFPS()
{
	mainGraphics->endTime = SDL_GetTicks();
	mainGraphics->numFrames++;

	//limit the drawing of fps
	if ((mainGraphics->endTime - mainGraphics->startTime > 1000) 
		&& mainGraphics->numFrames > 10)
	{
		double fps = (double)mainGraphics->numFrames / (mainGraphics->endTime / mainGraphics->startTime);
		//update our starting time again
		mainGraphics->startTime = mainGraphics->endTime;
		//reset the frames
		mainGraphics->numFrames = 0;

		std::stringstream ss;
		ss << mainGraphics->title;
		ss << " | ";
		ss << fps;
		ss << "fps";
		SDL_SetWindowTitle(mainGraphics->window, ss.str().c_str());
	}
}

void graphics_destroy()
{
	SDL_GL_DeleteContext(mainGraphics->glContext);
	log_fprint("Destroyed GL context");
	SDL_DestroyWindow(mainGraphics->window);
	log_fprint("Destroyed Window");
	delete mainGraphics;
	SDL_Quit();
}