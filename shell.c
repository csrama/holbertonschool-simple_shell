#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *prog_name = "hsh";
unsigned int line_number = 0;

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
		return 127;
	}

	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		free(cmd_path);
		return 1;
	}

	if (pid == 0) /* child */
	{
		execve(cmd_path, args, environ);
		perror(prog_name);
		free(cmd_path);
		_exit(126);
	}

	/* parent */
	waitpid(pid, &status, 0);
	free(cmd_path);
	return 0;
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	int i, j;

	while (1)
	{
		line_number++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(0);
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* skip empty lines */
		for (i = 0; line[i] != '\0'; i++)
			if (line[i] != ' ' && line[i] != '\t')
				break;
		if (line[i] == '\0')
			continue;

		/* tokenize line into args */
		i = 0;
		j = 0;
		while (line[j] != '\0')
		{
			while (line[j] == ' ' || line[j] == '\t')
				j++;
			if (line[j] == '\0')
				break;

			args[i++] = &line[j];

			while (line[j] != '\0' && line[j] != ' ' && line[j] != '\t')
				j++;
			if (line[j] != '\0')
			{
				line[j] = '\0';
				j++;
			}
		}
		args[i] = NULL;

		execute_command(args);
	}

	free(line);
	return 0;
}
