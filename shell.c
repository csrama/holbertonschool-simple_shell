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
	int i, j;
	pid_t pid;
	int status;

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

		/* Check if line has any non-space characters */
		for (i = 0; line[i] != '\0'; i++)
			if (line[i] != ' ' && line[i] != '\t')
				break;
		
		if (line[i] == '\0')
			continue;

		/* Split into arguments */
		i = 0;
		j = 0;
		while (line[j] != '\0')
		{
			/* Skip spaces */
			while (line[j] == ' ' || line[j] == '\t')
				j++;
			
			if (line[j] == '\0')
				break;
			
			args[i] = &line[j];
			i++;
			
			/* Find end of argument */
			while (line[j] != '\0' && line[j] != ' ' && line[j] != '\t')
				j++;
			
			if (line[j] != '\0')
			{
				line[j] = '\0';
				j++;
			}
		}
		args[i] = NULL;

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
