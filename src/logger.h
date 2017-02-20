#ifndef LOGGER_H
#define LOGGER_H

#if ANKH2D_WIN32
	#include <stdio.h>
	#include <stdarg.h>

	#include "config.h"

	//the log name
	static const char* log_name = "Ankh2D.log";

	//clears the file for writing
	void log_fclear();
	//outputting to a file
	void log_fprint(const char *fmt, ...);
	//printing to screen/console
	void log_print(char* _error);
#endif

#endif