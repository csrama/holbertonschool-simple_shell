#include "shell.h"
#include <sys/stat.h>


/**
 * get_path - extracts the PATH string from the environment
 * Return: pointer to the PATH value string, or NULL
 */
char *get_path(void)
{
	int i;

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
	}
	return (NULL);
}

/**
 * find_path - finds the full path of a command
 * @command: the command to find (e.g., "ls")
 * Return: full path string (must be freed), or NULL if not found
 */
char *find_path(const char *command)
{
	char *path, *path_copy, *dir, *full;
	struct stat st;

	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}
	path = get_path();
	if (!path || strlen(path) == 0)
		return (NULL);
	path_copy = strdup(path);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full = malloc(strlen(dir) + strlen(command) + 2);
		if (!full)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full, "%s/%s", dir, command);
		if (stat(full, &st) == 0)
		{
			free(path_copy);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
int resolve_command(char *cmd, char *full, size_t size, char **envp)
{
    char *p;

    (void)envp;
    p = find_path(cmd);
    if (!p)
        return (0);

    strncpy(full, p, size - 1);
    full[size - 1] = '\0';
    free(p);
    return (1);
}
