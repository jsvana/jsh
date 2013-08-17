#include "find.h"

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BLOCK_SIZE 256

char *find(char *dir, char *binary) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL) {
		fprintf(stderr, "Cannot open directory: %s\n", dir);
		return NULL;
	}

	chdir(dir);

	while ((entry = readdir(dp)) != NULL) {
		printf("ent: %s\n", entry->d_name);
		lstat(entry->d_name, &statbuf);

		if (S_ISDIR(statbuf.st_mode)) {
			printf("dir\n");
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
				continue;
			}

			char *found = find(entry->d_name, binary);
			if (found != NULL) {
				chdir("..");
				closedir(dp);

				return found;
			}
			chdir("..");
		} else {
			if (strcmp(binary, entry->d_name) == 0) {
				char *cwd = getcwd(NULL, 0);
				char *result = (char *)malloc(sizeof(char) *
					(strlen(cwd) + strlen(binary) + 1));

				strcpy(result, cwd);
				strcat(result, "/");
				strcat(result, binary);

				free(cwd);

				closedir(dp);

				return result;
			}
		}
	}

	return NULL;
}
