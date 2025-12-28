#include "shell.h"
#include "main.h"

/* Add this function or modify existing execute_command */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *full_path;

    /* Check if command exists before forking */
    full_path = find_command_in_path(args[0]);
    if (!full_path)
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
        return (127); /* Command not found */
    }

    /* Now we can safely fork */
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(full_path);
        return (1);
    }
    else if (pid == 0) /* Child */
    {
        if (execve(full_path, args, environ) == -1)
        {
            perror("execve");
            free(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else /* Parent */
    {
        waitpid(pid, &status, 0);
        free(full_path);
        
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else
            return (1);
    }

    return (0);
}
