#include "find.h"
#include "logging.h"
#include "signals.h"

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define INPUT_BUFFER_SIZE 8

/**
 * Fetches line of input from stdin
 * input must be freed before use
 * @param input Pointer to string to be allocated
 * @return Length of read string
 */
int getInput(char **input) {
	char buffer[INPUT_BUFFER_SIZE];
	int read = 0, totalRead = 0;
	*input = (char *)malloc(sizeof(char) * INPUT_BUFFER_SIZE);

	if (*input == NULL) {
		term_err("Unable to allocate memory");
	}

	memset(*input, 0, sizeof(char) * INPUT_BUFFER_SIZE);

	while (((*input)[totalRead] = getc(stdin)) != '\n') {
		++read;
		++totalRead;
		if (read == INPUT_BUFFER_SIZE) {
			*input = (char *)realloc(*input,
				sizeof(char) * INPUT_BUFFER_SIZE + totalRead);
			memset(*input + totalRead, 0, INPUT_BUFFER_SIZE);
			read = 0;
		}
	}

	(*input)[totalRead] = '\0';

	return totalRead;
}

int main(int argc, char *argv[]) {
	char *input;

	signal(SIGINT, handler);
	fprintf(stdout, ": ");
	int len = getInput(&input);
	char *path = find("/usr/include", input);

	if (path == NULL) {
		printf("Not found\n");
	} else {
		printf("path: %s\n", path);
	}

	free(input);
	free(path);

	return 0;
}
