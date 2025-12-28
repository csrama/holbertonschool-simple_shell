#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* execute command using fork and execve */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args || !args[0])
		return 0;

	cmd_path = find_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: %u: %s: not found\n",
			prog_name, line_number, args[0]);

		if (!isatty(STDIN_FILENO))
			_exit(127);  /* non-interactive mode */

		return 0;  /* interactive mode: print error only */
	}

	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, args, environ);
		perror(prog_name);
		_exit(1);
	}
	else if (pid > 0)
		wait(&status);

	free(cmd_path);
	return 0;
}

