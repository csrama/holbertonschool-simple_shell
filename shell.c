#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

int main(void)
{
	char line[1024];
	char *clean;
	char *args[2];
	pid_t pid;
	int status;
	int i, j;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		if (fgets(line, sizeof(line), stdin) == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove newline */
		line[strcspn(line, "\n")] = '\0';

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

		args[0] = clean;
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
				fprintf(stderr, "./hsh: 1: %s: not found\n", clean);
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
