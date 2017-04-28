#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef DEBUG
static inline void debug(const char *format, ...)
{}
#else
static inline void debug(const char *format, ...)
{
	if (DEBUG) {
		va_list argp;

		va_start(argp, format);
		vprintf(format, argp);
		va_end(argp);
	}
}
#endif /* DEBUG */
#endif /* DEBUG_H */
