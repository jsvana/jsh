#ifndef LOGGING_H
#define LOGGING_H

/**
 * Logs a value to stdout
 * @param fmt String to log
 * @param ... Format arguments
 */
void term_log(const char *fmt, ...);

/**
 * Logs an error to stderr
 * @param fmt String to log
 * @param ... Format arguments
 */
void term_err(const char *fmt, ...);

#endif
