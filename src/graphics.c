#include "graphics.h"

graphics graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs)
{
	//clear previous logs
	log_fclear();

	graphics main_graphics;

	main_graphics.title = _title;
	main_graphics.width = _width;
	main_graphics.height = _height;

	//setup SDL+GL windows and contexts
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	//disable vsync
	SDL_GL_SetSwapInterval(0);

	if (_fs)
	{
		main_graphics.window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			main_graphics.width, main_graphics.height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	}

	else
	{
		main_graphics.window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			main_graphics.width, main_graphics.height, SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	}

	main_graphics.gl_context = SDL_GL_CreateContext(main_graphics.window);

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
	main_graphics.start_time = SDL_GetTicks();

	return main_graphics;
}

void graphics_clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void graphics_update(graphics* _graphics)
{
	SDL_GL_SwapWindow(_graphics->window);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			_graphics->closed = true;

		if (event.key.keysym.sym == SDLK_ESCAPE)
			_graphics->closed = true;
	}

	graphics_get_fps(_graphics);
}

void graphics_get_fps(graphics* _graphics)
{
	_graphics->end_time = SDL_GetTicks();
	_graphics->num_frames++;

	//limit the drawing of fps
	if ((_graphics->end_time - _graphics->start_time > 1000) 
		&& _graphics->num_frames > 10)
	{
		double fps = (double)_graphics->num_frames / (_graphics->end_time / _graphics->start_time);
		//update our starting time again
		_graphics->start_time = _graphics->end_time;
		//reset the frames
		_graphics->num_frames = 0;

		std::stringstream ss;
		ss << _graphics->title;
		ss << " | ";
		ss << fps;
		ss << "fps";
		SDL_SetWindowTitle(_graphics->window, ss.str().c_str());
	}
}

void graphics_destroy(graphics* _graphics)
{
	SDL_GL_DeleteContext(_graphics->gl_context);
	log_fprint("Destroyed GL context");
	SDL_DestroyWindow(_graphics->window);
	log_fprint("Destroyed Window");
	SDL_Quit();
}