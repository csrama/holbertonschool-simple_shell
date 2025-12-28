#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *prog_name;
unsigned int line_number = 0;

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	int i;

	(void)argc;
	prog_name = argv[0];

	while (1)
	{
		line_number++;

		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			free(line);
			exit(0);
		}

		line[strcspn(line, "\n")] = '\0';

		i = 0;
		args[i] = strtok(line, " ");
		while (args[i])
			args[++i] = strtok(NULL, " ");

		if (args[0])
			execute_command(args);
	}
	return (0);
}

