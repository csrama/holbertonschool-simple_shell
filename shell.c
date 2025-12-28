#include "shell.h"

/**
 * get_path - Returns default PATH
 *
 * Return: String with PATH directories separated by ':'
 */
char *get_path(void)
{
	int i = 0;

	if (!environ)
		return (NULL);

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
		i++;
	}
	return (NULL);
}

/**
 * find_path - Finds command in PATH
 * @command: Command name
 *
 * Return: Full path or NULL
 */
char *find_path(char *command)
{
	char *path, *dir, *full, *path_copy;
	struct stat st;

	if (!command)
		return (NULL);

	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	path = get_path();
	if (!path || *path == '\0')
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		full = malloc(strlen(dir) + strlen(command) + 2);
		if (!full)
			break;

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

/**
 * execute_command - Executes command if found
 * @args: Argument list
 */
void execute_command(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args || !args[0])
		return;

	cmd_path = find_path(args[0]);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "hsh: command not found\n", 23);
		return;
		fprintf(stderr, "%s: %u: %s: not found\n",
				prog_name, line_number, args[0]);
		exit(127);
	}

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, environ);
		perror("hsh");
		exit(EXIT_FAILURE);
		perror(prog_name);
		exit(1);
	}
	else
		wait(&status);

	free(cmd_path);
}
