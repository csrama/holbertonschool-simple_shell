#include "shell.h"

/**
 * trim_spaces - Removes leading and trailing spaces from a string
 * @str: The string to trim
 *
 * Return: Pointer to the trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	/* Trim leading space */
	while (isspace((unsigned char)*str))
		str++;

	if (*str == 0)  /* All spaces? */
		return (str);

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;

	/* Write new null terminator */
	*(end + 1) = '\0';

	return (str);
}

/**
 * display_prompt - Displays the shell prompt
 *
 * Return: void
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * execute_command - Executes a command using execve
 * @command: The command to execute
 *
 * Return: void
 */
void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *args[2];

	/* Trim spaces from command */
	command = trim_spaces(command);

	/* Skip if command is empty after trimming */
	if (strlen(command) == 0)
		return;

	/* Prepare arguments for execve */
	args[0] = command;
	args[1] = NULL;

	/* Create child process */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}

	if (pid == 0)  /* Child process */
	{
		/* Execute command */
		if (execve(args[0], args, environ) == -1)
		{
			/* Print error like sh: ./hsh: 1: command: not found */
			fprintf(stderr, "./hsh: 1: %s: not found\n", command);
			exit(127); /* Exit code 127 for command not found */
		}
	}
	else  /* Parent process */
	{
		/* Wait for child to complete */
		wait(&status);
	}
}

/**
 * main - Entry point of simple shell
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *trimmed_line;

	while (1)
	{
		/* Display prompt */
		display_prompt();

		/* Read command from user */
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

		/* Trim spaces from the line */
		trimmed_line = trim_spaces(line);

		/* Skip empty lines */
		if (strlen(trimmed_line) == 0)
			continue;

		/* Execute command */
		execute_command(trimmed_line);
	}

	/* Free allocated memory */
	free(line);
	return (0);
}
