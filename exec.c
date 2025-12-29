#include "shell.h"

int run_external(char **argv, char **envp, const char *progname, int cmd_no)
{
    char full[1024];
    pid_t pid;
    int status;
    char **real_env;

    if (!argv || !argv[0])
        return 0;

    /* Do NOT fork if command doesn't exist */
    if (!resolve_command(argv[0], full, sizeof(full), envp))
    {
        fprintf(stderr, "%s: %d: %s: not found\n",
                progname ? progname : "hsh", cmd_no, argv[0]);
	return (127);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    real_env = envp ? envp : environ;

    if (pid == 0)
    {
        execve(full, argv, real_env);
        perror(progname ? progname : "hsh");
        _exit(126);
    }

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return 1;
}
