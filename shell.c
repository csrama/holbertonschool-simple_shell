#include "shell.h"

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
			fprintf(stderr, "./shell: No such file or directory\n");
			exit(1);
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

		/* Skip empty lines */
		if (strlen(line) == 0)
			continue;

		/* Execute command */
		execute_command(line);
	}

	/* Free allocated memory */
	free(line);
	return (0);
}
