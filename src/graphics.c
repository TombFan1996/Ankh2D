#include "graphics.h"

graphics* graphics_create(const char* _title, uint16_t _width, uint16_t _height, bool _fs)
{
	//clear previous logs
	log_fclear();

	graphics* main_graphics = (graphics*)malloc(sizeof(graphics));

	main_graphics->title = _title;
	main_graphics->width = _width;
	main_graphics->height = _height;

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

		main_graphics->fullscreen = _fs;

		if (_fs)
			main_graphics->window = glfwCreateWindow(main_graphics->width, main_graphics->height, main_graphics->title, glfwGetPrimaryMonitor(), NULL);

		else
			main_graphics->window = glfwCreateWindow(main_graphics->width, main_graphics->height, main_graphics->title, NULL, NULL);

		if (!main_graphics->window)
			log_fprint("ERROR: Failed to create Glfw window");
		
		else
		{
			//create the GL context
			glfwMakeContextCurrent(main_graphics->window);
			//setup error callback
			glfwSetErrorCallback(graphics_error_callback);
			//setup keyboard callback
			glfwSetKeyCallback(main_graphics->window, graphics_input_callback);
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

	main_graphics->closed = false;

	main_graphics->num_frames = 0;
	
	//get the starting time
	main_graphics->start_time = glfwGetTime();

	log_fprint("glfw successfully created");

	return main_graphics;
}

void graphics_set_swap_interval(bool _vsync)
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

void graphics_update(graphics* _graphics)
{
	glfwPollEvents();
	glfwSwapBuffers(_graphics->window);
}

void graphics_get_fps(graphics* _graphics, double* _fps)
{
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
}

void graphics_destroy(graphics* _graphics)
{
	glfwDestroyWindow(_graphics->window);
	log_fprint("destroyed glfw window");
	glfwTerminate();
	log_fprint("destroyed glfw");
}