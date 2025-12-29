#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_command(char **args)
{
    pid_t pid;
    char *cmd_path;
    char *path;

    if (!args || !args[0])
        return 0;

    path = get_path_value();
    if (path && path[0] == '\0')
    {
        fprintf(stderr, "%s: %u: %s: not found\n",
                prog_name, line_number, args[0]);
        return 127;
    }

    cmd_path = find_path(args[0]);
    if (!cmd_path)
    {
        fprintf(stderr, "%s: %u: %s: not found\n",
                prog_name, line_number, args[0]);
        return 127;
    }

    pid = fork();
    if (pid == -1)
    {
        perror(prog_name);
        free(cmd_path);
        return 1;
    }

    if (pid == 0)
    {
        execve(cmd_path, args, environ);
        perror(prog_name);
        free(cmd_path);
        _exit(126);
    }

    wait(NULL);
    free(cmd_path);
    return 0;
}

