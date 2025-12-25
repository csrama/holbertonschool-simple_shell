#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_PATH_LEN 4096

/**
 * split_string - Splits a string into tokens
 * @str: The string to split
 * @delim: The delimiter character
 * Return: Array of tokens, NULL terminated
 */
char **split_string(char *str, const char *delim)
{
    char **tokens = malloc(MAX_ARGS * sizeof(char *));
    char *token;
    int i = 0;

    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(str, delim);
    while (token != NULL && i < MAX_ARGS - 1)
    {
        tokens[i] = token;
        i++;
        token = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    return tokens;
}

/**
 * get_path_dirs - Gets all directories in PATH
 * Return: Array of PATH directories, NULL terminated
 */
char **get_path_dirs(void)
{
    char *path_env = getenv("PATH");
    char *path_copy;
    char **dirs;

    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
    {
        perror("strdup");
        return NULL;
    }

    dirs = split_string(path_copy, ":");
    free(path_copy);
    return dirs;
}

/**
 * find_command_in_path - Searches for command in PATH directories
 * @command: The command to find
 * Return: Full path if found, NULL otherwise
 */
char *find_command_in_path(char *command)
{
    char **path_dirs;
    char *full_path = NULL;
    struct stat st;
    int i = 0;

    /* If command contains '/', check it directly */
    if (strchr(command, '/'))
    {
        if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
            return strdup(command);
        return NULL;
    }

    path_dirs = get_path_dirs();
    if (!path_dirs)
        return NULL;

    while (path_dirs[i])
    {
        full_path = malloc(strlen(path_dirs[i]) + strlen(command) + 2);
        if (!full_path)
        {
            perror("malloc");
            free(path_dirs);
            return NULL;
        }

        sprintf(full_path, "%s/%s", path_dirs[i], command);

        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            free(path_dirs);
            return full_path;
        }

        free(full_path);
        i++;
    }

    free(path_dirs);
    return NULL;
}

/**
 * execute_command - Executes a command with arguments
 * @args: Array of command and arguments
 * Return: 1 to continue, 0 to exit
 */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *full_path;

    if (!args || !args[0])
        return 1;

    /* Handle exit command */
    if (strcmp(args[0], "exit") == 0)
        return 0;

    /* Handle env command */
    if (strcmp(args[0], "env") == 0)
    {
        extern char **environ;
        char **env = environ;
        while (*env)
        {
            printf("%s\n", *env);
            env++;
        }
        return 1;
    }

    /* Find command in PATH */
    full_path = find_command_in_path(args[0]);
    if (!full_path)
    {
        fprintf(stderr, "./shell: No such file or directory\n");
        return 1;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(full_path);
        return 1;
    }

    if (pid == 0)
    {
        /* Child process */
        if (execve(full_path, args, environ) == -1)
        {
            perror("./shell");
            free(full_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        waitpid(pid, &status, 0);
        free(full_path);
    }

    return 1;
}

/**
 * main - Main shell loop
 * Return: Always 0
 */
int main(void)
{
    char input[MAX_INPUT];
    char **args;
    int status = 1;

    while (status)
    {
        printf(":) ");
        
        if (!fgets(input, MAX_INPUT, stdin))
        {
            printf("\n");
            break;
        }

        /* Remove newline character */
        input[strcspn(input, "\n")] = 0;

        /* Skip empty input */
        if (strlen(input) == 0)
            continue;

        args = split_string(input, " ");
        status = execute_command(args);
        free(args);
    }

    return 0;
}
