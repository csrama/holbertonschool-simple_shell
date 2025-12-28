#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * execute_command - fork and execute command
 * @args: arguments array
 * Return: 0
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args || !args[0])
		return (0);

	cmd_path = find_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "hsh: %s: not found\n", args[0]);
		return (0);
	}

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, environ);
		perror("hsh");
		exit(EXIT_FAILURE);
	}
	else
		wait(&status);

	free(cmd_path);
	return (0);
}

