#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *prog_name;
unsigned int line_number;

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	int i, status = 0;

	(void)argc;
	prog_name = argv[0];
	line_number = 0;

	while (1)
	{
		line_number++;

		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			free(line);
			exit(status);
		}

		line[strcspn(line, "\n")] = '\0';

		i = 0;
		args[i] = strtok(line, " \t");
		while (args[i])
			args[++i] = strtok(NULL, " \t");

		/* ✅ EXIT BUILT-IN — MUST BE HERE */
		if (args[0] && strcmp(args[0], "exit") == 0)
		{
			free(line);
			exit(0);
		}

		if (args[0])
			status = execute_command(args);
	}
}
