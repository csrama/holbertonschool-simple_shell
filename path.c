#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "shell.h"

/**
	* search_in_path - search command in PATH
		* @cmd: command name
	* @path: PATH value
	*
	* Return: full path or NULL
	*/
static char *search_in_path(char *cmd, char *path)
{
@@ -36,21 +55,22 @@
	free(full);
	token = strtok(NULL, ":");
	}
	free(copy);
	return (NULL);
}

/**
	* find_command - find executable using PATH
	* @cmd: command name
	* @ctx: shell context
	*
	* Return: full path or NULL
	*/
char *find_command(char *cmd, shell_ctx_t *ctx)
{
	struct stat st;
	char *path;

	(void)ctx;

@@ -64,9 +84,9 @@
	return (NULL);
	}

	path = getenv("PATH");
	if (!path || path[0] == '\0')
	return (NULL);

	return (search_in_path(cmd, path));
}
