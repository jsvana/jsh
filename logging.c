#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void term_log(const char *fmt, ...) {
	struct timeval t; \
	gettimeofday(&t, NULL); \
	fprintf(stdout, "[LOG %d.%d:%d] ", (int)t.tv_sec, (int)t.tv_usec, __LINE__);

	va_list args;

	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
}

void term_err(const char *fmt, ...) {
	struct timeval t; \
	gettimeofday(&t, NULL); \
	fprintf(stdout, "[ERROR %d.%d:%d] ", (int)t.tv_sec, (int)t.tv_usec, __LINE__);

	va_list args;

	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);

	exit(1);
}
