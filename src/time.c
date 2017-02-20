#include "time.h"

#if ANKH2D_WIN32
	time time_create()
	{
		time time_new;
		time_new.start_time = glfwGetTime();
		time_new.end_time = 0;
		time_new.delta_time = 0.0f;
		return time_new;
	}

	void time_update_deltatime(time* _timer)
	{
		_timer->end_time = glfwGetTime();
		_timer->delta_time = _timer->end_time - _timer->start_time;
		_timer->start_time = _timer->end_time;
	}
#elif ANKH2D_PSX

#endif