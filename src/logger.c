#include "logger.h"

#if ANKH2D_WIN32
	void log_fclear()
	{
		FILE* file;
		file = fopen(log_name, "w");
		fclose(file);
	}

	void log_fprint(const char *fmt, ...)
	{
		FILE* file;
		file = fopen(log_name, "a");

		char text[256];
		va_list ap;

		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
		
		fprintf(file, text);
		fprintf(file, "\n");
		fclose(file);
	}

#endif