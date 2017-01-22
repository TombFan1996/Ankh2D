#include "logger.h"

void log_fclear()
{
	FILE* file;
	file = fopen(log_name, "w");
	fclose(file);
}

void log_fprint(const char *fmt, ...)
{
	//only write if we're in debug
	#ifdef ANKH2D_DEBUG
		FILE* file;
		file = fopen(log_name, "a");

		char text[1024];
		va_list ap;

		if (fmt == NULL)
    		*text=0;

		else 
		{	
			va_start(ap, fmt);
			vsprintf(text, fmt, ap);
			va_end(ap);
		}

		fprintf(file, text);
		fprintf(file, "\n");
		fclose(file);
	#endif
}