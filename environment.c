#include "environment.h"

#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int environmentInit(environment *env, char **envp) {
	int place;

	env->count = 0;
	env->keys = (char **)malloc(sizeof(char *) * 64);
	env->variables = envp;

	for (int i = 0; env->variables[i] != NULL; i++) {
		place = 0;

		while (env->variables[i][place] != '=') {
			++place;
		}

		env->keys[i] = (char *)malloc(sizeof(char) * place);
		strncpy(env->keys[i], env->variables[i], place);

		++env->count;
	}

	return TRUE;
}

char *getEnv(environment env, const char *key) {
	for (int i = 0; i < env.count; i++) {
		if (strcmp(key, env.keys[i]) == 0) {
			return env.variables[i];
		}
	}

	return "";
}

char *getValue(const char *var) {
	char *loc = strchr(var, '=');

	if (loc == '\0') {
		return "";
	} else {
		return loc + 1;
	}
}

void printEnv(environment env) {
	for (int i = 0; i < env.count; i++) {
		fprintf(stdout, "%s\n", env.variables[i]);
	}
}
