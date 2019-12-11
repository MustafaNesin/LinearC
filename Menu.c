#include <stdio.h>
#include <stdarg.h>
#include "Main.h"

int mf_back(MxMemory* memory)
{
	return 0;
}

int mf_error(char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	get_char();
	return 1;
}