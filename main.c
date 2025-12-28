#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * main - entry point of the simple shell
 * Return: 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	int i;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			free(line);
			exit(0);
		}

		/* remove newline */
		line[strcspn(line, "\n")] = '\0';

		i = 0;
		args[i] = strtok(line, " ");
		while (args[i])
		{
			i++;
			args[i] = strtok(NULL, " ");
		}

		if (args[0])
			execute_command(args);
	}
	return (0);
}

