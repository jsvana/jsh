#include "command.h"

#include "defines.h"
#include "find.h"
#include "logging.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern int termRunning;
extern char *cwd;
extern int lastStatus;
extern int cpid;

int process(int argc, char **argv, environment env) {
	char *command = argv[0];

	if (argc == 0) {
		return lastStatus;
	} else if (strlen(command) > 2 && command[0] == '.' && command[1] == '/') {
		char *path;
		asprintf(&path, "%s/%s", cwd, command);
		if (path == NULL) {
			TERM_ERR("Unable to allocate asprintf memory\n");
		}

		cpid = fork();
		if (cpid < 0) {
			TERM_ERR("Unable to fork\n");
		} else if (cpid == 0) {
			execve(path, argv, env.variables);
		} else {
			int status;
			wait(&status);
			if (WIFEXITED(status)) {
				return WEXITSTATUS(status) == 0;
			} else {
				return FALSE;
			}
		}
		free(path);
		cpid = -1;
	} else if (strcmp(command, "exit") == 0) {
		fprintf(stdout, "Exiting...\n");
		termRunning = FALSE;

		return TRUE;
	} else if (strcmp(command, "cd") == 0) {
		if (argc == 1) {
			free(cwd);

			char *home = getValue(getEnv(env, "HOME"));

			if (chdir(home) != -1) {
				cwd = (char *)malloc(sizeof(char) * (strlen(home) + 1));
				memset(cwd, 0, sizeof(char) * (strlen(home) + 1));
				strcpy(cwd, home);

				return TRUE;
			} else {
				return FALSE;
			}
		} else {
			free(cwd);
			if (chdir(argv[1]) != -1) {
				cwd = getcwd(NULL, 0);

				return TRUE;
			} else {
				return FALSE;
			}
		}
	} else if (strcmp(command, "pwd") == 0) {
		char *cwd = getcwd(NULL, 0);
		fprintf(stdout, "%s\n", cwd);
		free(cwd);

		return TRUE;
	} else if (strcmp(command, "env") == 0) {
		printEnv(env);

		return TRUE;
	} else {
		char *path = find(getValue(getEnv(env, "PATH")), command);

		if (path != NULL) {
			cpid = fork();
			if (cpid < 0) {
				TERM_ERR("Unable to fork\n");
			} else if (cpid == 0) {
				execve(path, argv, env.variables);
			} else {
				int status;
				wait(&status);
				if (WIFEXITED(status)) {
					return WEXITSTATUS(status) == 0;
				} else {
					return FALSE;
				}
			}
			free(path);
			cpid = -1;

			return TRUE;
		}
	}

	fprintf(stderr, "jsh: Unknown command \"%s\"\n", command);

	return FALSE;
}
