#ifndef TIME_H
#define TIME_H

#include <GLFW\glfw3.h>
#include <stdint.h>

typedef struct
{
	double start_time, end_time;
	double delta_time;
} time;

time time_create();
void time_update_deltatime(time* _timer);

#endif