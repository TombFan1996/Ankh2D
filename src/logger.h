#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

//the log name
static const char* log_name = "Ankh2D.log";

//clears the file for writing
void log_fclear();
//outputting to a file
void log_fprint(const char *fmt, ...);
//printing to screen/console
void log_print(char* _error);

#endif