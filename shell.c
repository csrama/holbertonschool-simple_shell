#include "shell.h"

char *prog_name;
unsigned int line_number;

/**
 * execute_command - Forks and executes a command if found
 * @args: array of arguments
 * Return: status code
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	cmd_path = find_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: %u: %s: not found\n",
				prog_name, line_number, args[0]);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			perror(prog_name);
			free(cmd_path);
			exit(127);
		}
	}
	else
	{
		wait(&status);
		free(cmd_path);
	}
	return (0);
}

/**
 * main - entry point for the simple shell
 * @ac: arg count
 * @av: arg vector
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *args[64];
	int i;
	(void)ac;

	prog_name = av[0];
	line_number = 0;

	while (1)
	{
		line_number++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		i = 0;
		args[i] = strtok(line, " \t");
		while (args[i])
			args[++i] = strtok(NULL, " \t");

		if (args[0])
			execute_command(args);
	}
	free(line);
	return (0);
}
