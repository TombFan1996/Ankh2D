#ifndef TIME_H
#define TIME_H

#if ANKH2D_WIN32
	#include <GLFW\glfw3.h>
	#include <stdint.h>

	typedef struct
	{
		double start_time, end_time;
		double delta_time;
	} time;

	time time_create();
	void time_update_deltatime(time* _timer);

#elif ANKH2D_PSX

#endif

#endif