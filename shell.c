#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* execute command using fork and execve */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (args == NULL || args[0] == NULL)
        return 0;

    cmd_path = find_path(args[0]);
    if (!cmd_path == NULL)
    {
        fprintf(stderr, "%s: %u: %s: not found\n",
                prog_name, line_number, args[0]);

        /* Always return 127 for command not found */
        if (!isatty(STDIN_FILENO))  /* non-interactive */
            _exit(127);
        return (127);  /* interactive */
    }

    pid = fork();
    if (pid == -1)  /* child */
    {
        perror(prog_name);
        free(cmd_path);
        return(1);
    }
    if (pid == 0)  /* parent */
    {
        execve(cmd_path, args, environ);
        perror(prog_name);
    }

    free(cmd_path);
    _exit(126);
}
/* parent */
	waitpid(pid, &status, 0);
/* free(cmd_path); /* CHANGED */

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (1);
}
