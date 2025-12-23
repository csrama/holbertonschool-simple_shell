#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * trim_spaces - Remove spaces from start and end
 * @str: String to trim
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
		if (interactive)
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		if (fgets(buffer, sizeof(buffer), stdin) == NULL)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove newline */
		buffer[strcspn(buffer, "\n")] = '\0';

		/* Trim spaces */
		trimmed = trim_spaces(buffer);

		/* Skip if empty after trimming */
		if (strlen(trimmed) == 0)
			continue;

		args[0] = trimmed;
		args[1] = NULL;

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
				fprintf(stderr, "./hsh: 1: %s: not found\n", trimmed);
				exit(127);
			}
		}
		else
		{
			wait(&status);
		}
	}

	return (0);
}
