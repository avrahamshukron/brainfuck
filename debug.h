#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>

#ifndef DEBUG
#define DEBUG 0
#endif

static inline void vdebug(const char *format, va_list argp)
{
	vprintf(format, argp);
}

static inline void debug(const char *format, ...)
{
	if (DEBUG) {
		va_list argp;
		va_start(argp, format);
		vdebug(format, argp);
		va_end(argp);
	}
}

#endif /* DEBUG_H */
