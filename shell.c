#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * main - Simple shell 0.1
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	char *token;
	int i;
	pid_t pid;
	int status;

	while (1)
	{
		/* Display prompt if interactive */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		/* Read command using getline */
		read = getline(&line, &len, stdin);

		/* Handle Ctrl+D (EOF) */
		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(0);
		}

		/* Remove newline character */
		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* Skip empty lines */
		if (strlen(line) == 0)
			continue;

		/* Split command into arguments */
		i = 0;
		token = strtok(line, " ");
		while (token != NULL && i < 63)
		{
			args[i] = token;
			i++;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;  /* NULL terminate */

		/* Fork child process */
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			continue;
		}

		if (pid == 0)  /* Child process */
		{
			/* Execute command with arguments */
			if (execve(args[0], args, environ) == -1)
			{
				fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
				exit(127);
			}
		}
		else  /* Parent process */
		{
			/* Wait for child to complete */
			wait(&status);
		}
	}

	/* Free allocated memory */
	free(line);
	return (0);
}
