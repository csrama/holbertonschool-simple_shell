#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * trim_spaces - Remove leading and trailing spaces
 * @str: String to trim
 *
 * Return: Trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	/* Remove leading spaces */
	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == 0)
		return (str);

	/* Remove trailing spaces */
	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * main - Simple shell 0.1
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char buffer[1024];
	char *trimmed;
	char *args[2];
	pid_t pid;
	int status;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		/* Display prompt only in interactive mode */
		if (interactive)
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		/* Read command */
		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		{
			/* Handle Ctrl+D */
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove newline */
		buffer[strcspn(buffer, "\n")] = '\0';

		/* Trim spaces from beginning and end */
		trimmed = trim_spaces(buffer);

		/* Skip empty lines */
		if (strlen(trimmed) == 0)
			continue;

		/* Prepare arguments for execve */
		args[0] = trimmed;
		args[1] = NULL;

		/* Fork process */
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
				fprintf(stderr, "./hsh: 1: %s: not found\n", trimmed);
				exit(127);
			}
		}
		else  /* Parent process */
		{
			/* Wait for child to complete */
			wait(&status);
		}
	}

	return (0);
}
