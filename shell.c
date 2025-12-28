#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/* execute command using fork and execve */
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
        /* Command not found */
        fprintf(stderr, "%s: %u: %s: not found\n",
                prog_name, line_number, args[0]);

        /* Non-interactive: exit with 127 */
        if (!isatty(STDIN_FILENO))
            _exit(127);

        /* Interactive: just print error, do NOT fork */
        return 0;
    }

    /* Only fork if command exists */
    pid = fork();
    if (pid == -1)
    {
        perror(prog_name);
        free(cmd_path);
        return 1;
    }

    if (pid == 0) /* Child process */
    {
        execve(cmd_path, args, environ);
        perror(prog_name);
        free(cmd_path);
        _exit(126);
    }

    /* Parent process */
    waitpid(pid, &status, 0);
    free(cmd_path);

    return 0;
}
