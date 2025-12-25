#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * find_command - find command in PATH
 * @cmd: command name
 * Return: full path or NULL
 */
char *find_command(char *cmd)
{
	char *path, *path_copy, *dir, *full_path;
	int cmd_len, dir_len;

	if (!cmd)
		return (NULL);

	/* Absolute or relative path */
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	path = getenv("PATH");
	if (!path || path[0] == '\0')
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	cmd_len = strlen(cmd);
	dir = strtok(path_copy, ":");

	while (dir)
	{
		dir_len = strlen(dir);
		full_path = malloc(dir_len + cmd_len + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}

		sprintf(full_path, "%s/%s", dir, cmd);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * main - simple shell
 * Return: 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *args[64];
	char *cmd_path;
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

		/* Ignore empty lines */
		for (i = 0; line[i]; i++)
			if (line[i] != ' ' && line[i] != '\t')
				break;
		if (!line[i])
			continue;

		/* Split input into arguments */
		i = 0;
		j = 0;
		while (line[j])
		{
			while (line[j] == ' ' || line[j] == '\t')
				j++;
			if (!line[j])
				break;

			args[i++] = &line[j];
			while (line[j] && line[j] != ' ' && line[j] != '\t')
				j++;
			if (line[j])
				line[j++] = '\0';
		}
		args[i] = NULL;

		/* Find command */
		cmd_path = find_command(args[0]);
		if (!cmd_path)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			continue; /* 🚫 NO fork */
		}

		pid = fork();
		if (pid == 0)
		{
			execve(cmd_path, args, environ);
			perror("execve");
			exit(1);
		}
		else if (pid > 0)
		{
			wait(&status);
		}

		free(cmd_path);
	}

	free(line);
	return (0);
}
