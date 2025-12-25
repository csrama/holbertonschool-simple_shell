#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "shell.h"

/**
	* search_in_path - search command in PATH
	* get_path - get the value of PATH from environment
	* @env: environment variables
	*
	* Return: pointer to PATH string or NULL if not found
	*/
char *get_path(char **env)
{
	int i;
	size_t len = 5;

	for (i = 0; env[i]; i++)
	{
	if (strncmp(env[i], "PATH=", len) == 0)
	return (env[i] + len);
	}
	return (NULL);
}

/**
	* search_in_path - search command in a PATH string
	* @cmd: command name
	* @path: PATH value
	*
	* Return: full path or NULL
	* Return: full path string or NULL
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
	* find_command - find executable path of a command
	* @cmd: command name
	* @ctx: shell context
	*
	* Return: full path or NULL
	* Return: full path string or NULL if not found
	*/
char *find_command(char *cmd, shell_ctx_t *ctx)
{
	struct stat st;
	char *path;
	struct stat st;

	(void)ctx;

@@ -64,9 +84,9 @@
	return (NULL);
	}

	path = getenv("PATH");
	path = get_path(ctx->env);
	if (!path || path[0] == '\0')
	return (NULL);

	return (search_in_path(cmd, path));
}
