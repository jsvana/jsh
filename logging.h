#ifndef LOGGING_H
#define LOGGING_H

#include <sys/time.h>

/**
 * MACRO
 * Logs a value to stdout
 * @param fmt String to log
 * @param ... Format arguments
 */
#define TERM_LOG(fmt, ...) \
	{ \
		struct timeval t; \
		gettimeofday(&t, NULL); \
		fprintf(stdout, "[LOG %d.%d %s:%d] ", (int)t.tv_sec, (int)t.tv_usec, \
			__FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); \
	}

/**
 * MACRO
 * Logs an error to stderr
 * @param fmt String to log
 * @param ... Format arguments
 */
#define TERM_ERR(fmt, ...) \
	{ \
		struct timeval t; \
		gettimeofday(&t, NULL); \
		fprintf(stdout, "[ERROR %d.%d %s:%d] ", (int)t.tv_sec, (int)t.tv_usec, \
			__FILE__, __LINE__); \
		fprintf(stdout, fmt, ##__VA_ARGS__); \
		exit(1); \
	}

#endif
