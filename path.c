#include "shell.h"
#include "main.h"

/**
 * find_command_in_path - Finds command in PATH directories
 * @command: Command to search for
 *
 * Return: Full path if found, NULL otherwise
 */
char *find_command_in_path(char *command)
{
    char *path, *path_copy, *dir, *full_path;
    struct stat st;
    int i;

    /* Check if command is already a path */
    if (strchr(command, '/'))
    {
        if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
            return (strdup(command));
        return (NULL);
    }

    /* Get PATH from environment */
    path = getenv("PATH");
    if (!path || strlen(path) == 0)
        return (NULL);

    /* Copy PATH for tokenization */
    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        /* Build full path */
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        if (!full_path)
        {
            free(path_copy);
            return (NULL);
        }

        sprintf(full_path, "%s/%s", dir, command);

        /* Check if file exists and is executable */
        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
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
 * check_command_exists - Checks if a command exists
 * @command: Command to check
 *
 * Return: 1 if exists, 0 otherwise
 */
int check_command_exists(char *command)
{
    char *full_path = find_command_in_path(command);
    if (full_path)
    {
        free(full_path);
        return (1);
    }
    return (0);
}
