#include "find.h"

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BLOCK_SIZE 256

extern char *cwd;

char *find(char *path, char *binary) {
	int len = strlen(path);

	if (len == 0) {
		return NULL;
	}

	int first = 0;
	int last = 0;

	char sub[BLOCK_SIZE];

	while (path[last] != '\0' && last < len) {
		while (path[last] != ':' && path[last] != '\0' && last < len) {
			++last;
		}

		memset(sub, 0, sizeof(char) * BLOCK_SIZE);

		strncpy(sub, path + first, last - first);

		char *found = findInDir(sub, binary);
		chdir(cwd);

		if (found != NULL) {
			return found;
		}

		first = last + 1;
		last = first;
	}

	return NULL;
}

char *findInDir(char *dir, char *binary) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL) {
		return NULL;
	}

	chdir(dir);

	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);

		if (S_ISDIR(statbuf.st_mode)) {
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
				continue;
			}

			char *found = findInDir(entry->d_name, binary);

			chdir("..");

			if (found != NULL) {
				closedir(dp);

				return found;
			}
		} else {
			if (strcmp(binary, entry->d_name) == 0) {
				char *cwd = getcwd(NULL, 0);
				char *result = (char *)malloc(sizeof(char) *
					(strlen(cwd) + strlen(binary) + 1));

				strcpy(result, cwd);
				strcat(result, "/");
				strcat(result, binary);

				free(cwd);

				chdir("..");
				closedir(dp);

				return result;
			}
		}
	}

	return NULL;
}
