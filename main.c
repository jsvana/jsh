#include "command.h"
#include "environment.h"
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

#define INPUT_BUFFER_SIZE 256
#define PROMPT ": "
#define MAX_PATH_LEN 256

int termRunning = TRUE;
char *cwd;

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
			memset(*input + totalRead, 0, sizeof(char) * INPUT_BUFFER_SIZE);
			read = 0;
		}
	}

	(*input)[totalRead] = '\0';

	return totalRead;
}

void parseInput(char *input, int *argc, char ***argv) {
	*argc = 0;
	int start = 0, end = 0;
	int len = strlen(input);

	*argv = NULL;

	while (end < len) {
		while (end < len && input[end] != ' ') {
			++end;
		}

		input[end] = '\0';

		++*argc;

		*argv = (char **)realloc(*argv, sizeof(char *) * *argc);

		int len = strlen(input + start) + 1;

		(*argv)[*argc - 1] = (char *)malloc(sizeof(char) * len);
		memset((*argv)[*argc - 1], 0, sizeof(char) * len);
		strncpy((*argv)[*argc - 1], input + start, end - start);

		start = end + 1;
	}

	*argv = (char **)realloc(*argv, sizeof(char *) * (*argc + 1));

	(*argv)[*argc] = NULL;
}

void cleanupCommand(int argc, char **argv) {
	for (int i = 0; i < argc; i++) {
		free(argv[i]);
	}

	free(argv);
}

void prompt() {
	fprintf(stdout, "%s: ", cwd);
}

int main(int argc, char **argv, char **envp) {
	char *input;
	int len, code;

	environment env;

	signalsInit();
	environmentInit(&env, envp);

	cwd = getcwd(NULL, 0);

	char p[MAX_PATH_LEN];
	int argC;
	char **argV;

	memset(p, 0, sizeof(char) * MAX_PATH_LEN);
	strncpy(p, "/bin:/usr/bin", MAX_PATH_LEN - 1);

	while (termRunning) {
		prompt();

		len = getInput(&input);
		parseInput(input, &argC, &argV);

		code = process(argC, argV, env);

		free(input);

		cleanupCommand(argC, argV);
	}

	return 0;
}
