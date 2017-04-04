#ifndef BTIME_H
#define BTIME_H

#if ANKH2D_WIN32
	#include <GLFW\glfw3.h>
	#include <stdint.h>

	typedef struct
	{
		double start_time, end_time;
		double delta_time;
	} btime;

	btime btime_create();
	void btime_update_deltatime(btime* _timer);

#elif ANKH2D_PSX

#endif

#endif