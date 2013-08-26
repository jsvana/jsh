#include "command.h"
#include "defines.h"
#include "environment.h"
#include "find.h"
#include "logging.h"
#include "signals.h"

#include <readline/readline.h>
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
int lastStatus;
char *cwd;
char *user;
int cpid = -1;

void parseInput(char *input, int *argc, char ***argv) {
	if (input == NULL) {
		return;
	}

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
		if ((*argv)[*argc - 1] == NULL) {
			TERM_ERR("Unable to allocate memory\n");
		}
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

char *prompt(environment env) {
	char *p;
	asprintf(&p, "%s%s%s: ", COLOR_RED, cwd, COLOR_WHITE);
	return p;
}

int main(int argc, char **argv, char **envp) {
	char *input;
	int len, code;

	environment env;

	signalsInit();
	environmentInit(&env, envp);

	using_history();

	cwd = getcwd(NULL, 0);
	user = getValue(getEnv(env, "USER"));
	lastStatus = TRUE;

	char p[MAX_PATH_LEN];
	int argC;
	char **argV;

	memset(p, 0, sizeof(char) * MAX_PATH_LEN);
	strncpy(p, "/bin:/usr/bin", MAX_PATH_LEN - 1);

	TERM_LOG("asdf %s\n", "fdsa");

	while (termRunning) {
		char *p = prompt(env);
		char *input = readline(p);
		if (input == NULL) {
			fprintf(stdout, "\n");
			termRunning = FALSE;
			break;
		}
		free(p);
		if (input != NULL && strlen(input) != 0) {
			add_history(input);
		}
		parseInput(input, &argC, &argV);

		lastStatus = process(argC, argV, env);

		free(input);

		cleanupCommand(argC, argV);
	}

	return 0;
}
