#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/* get PATH env variable */
char *get_path(void)
{
	int i;

	if (!environ)
		return NULL;

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return environ[i] + 5;
	}
	return NULL;
}

/* find full path of command */
char *find_path(char *command)
{
	char *path, *path_copy, *dir, *full;
	struct stat st;

	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return strdup(command);
		return NULL;
	}

	path = get_path();
	if (!path)
		return NULL;

	path_copy = strdup(path);
	if (!path_copy)
		return NULL;

	dir = strtok(path_copy, ":");
	while (dir)
	{
		full = malloc(strlen(dir) + strlen(command) + 2);
		if (!full)
		{
			free(path_copy);
			return NULL;
		}

		sprintf(full, "%s/%s", dir, command);

		if (stat(full, &st) == 0)
		{
			free(path_copy);
			return full;
		}

		free(full);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return NULL;
}

