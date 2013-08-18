#ifndef ENV_H
#define ENV_H

typedef struct {
	int count;
	char **keys;
	char **variables;
} environment;

int environmentInit(environment *env, char **envp);

char *getEnv(environment env, const char *key);
char *getValue(const char *var);

void printEnv(environment env);

#endif
