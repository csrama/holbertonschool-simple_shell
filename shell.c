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
	char *clean;
	char *args[2];
	pid_t pid;
	int status;
	int i;

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

		/* Remove leading spaces */
		clean = line;
		while (*clean == ' ' || *clean == '\t')
			clean++;

		/* Remove trailing spaces */
		i = strlen(clean) - 1;
		while (i >= 0 && (clean[i] == ' ' || clean[i] == '\t'))
			clean[i--] = '\0';

		/* Skip empty lines */
		if (strlen(clean) == 0)
			continue;

		/* Prepare arguments for execve */
		args[0] = clean;
		args[1] = NULL;

		/* Fork child process */
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			continue;
		}

		if (pid == 0)  /* Child process */
		{
			/* Execute command */
			if (execve(args[0], args, environ) == -1)
			{
				fprintf(stderr, "./hsh: 1: %s: not found\n", clean);
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
