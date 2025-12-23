#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	int i, j, start;
	pid_t pid;
	int status;
	int empty;

	while (1)
	{
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

		/* Check if line is empty or only spaces */
		empty = 1;
		for (i = 0; line[i] != '\0'; i++)
		{
			if (line[i] != ' ' && line[i] != '\t')
			{
				empty = 0;
				break;
			}
		}
		if (empty)
			continue;

		/* Split arguments */
		i = 0;
		j = 0;
		while (line[j] != '\0')
		{
			while (line[j] == ' ' || line[j] == '\t')
				j++;
			
			if (line[j] == '\0')
				break;
			
			args[i] = &line[j];
			i++;
			
			while (line[j] != '\0' && line[j] != ' ' && line[j] != '\t')
				j++;
			
			if (line[j] != '\0')
			{
				line[j] = '\0';
				j++;
			}
		}
		args[i] = NULL;

		if (args[0] == NULL)
			continue;

		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			continue;
		}

		if (pid == 0)
		{
			if (execve(args[0], args, environ) == -1)
			{
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				exit(127);
			}
		}
		else
		{
			wait(&status);
		}
	}

	free(line);
	return (0);
}
