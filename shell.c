#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

char *prog_name = NULL;
unsigned int line_number = 0;

int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (!args || !args[0])
        return 0;

    cmd_path = find_path(args[0]);
    if (!cmd_path)
    {
        fprintf(stderr, "%s: %u: %s: not found\n",
                prog_name, line_number, args[0]);

        if (!isatty(STDIN_FILENO)) /* non-interactive */
            _exit(127);
        return 127; /* interactive */
    }

    pid = fork();
    if (pid == 0) /* child */
    {
        execve(cmd_path, args, environ);
        perror(prog_name);
        _exit(1);
    }
    else if (pid > 0) /* parent */
    {
        wait(&status);
    }

    free(cmd_path);
    return WEXITSTATUS(status);
}

