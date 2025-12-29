#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

/* Find full path of a command */
char *find_path(const char *command)
{
    char *path_env, *path_copy, *dir, *full;
    struct stat st;

    if (!command)
        return NULL;

    /* If command contains '/' check directly */
    if (strchr(command, '/'))
    {
        if (stat(command, &st) == 0)
            return strdup(command);
        return NULL;
    }

    path_env = getenv("PATH");
    if (!path_env || path_env[0] == '\0')
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        full = malloc(strlen(dir) + strlen(command) + 2);
        if (!full)
        {
            free(path_copy);
            return NULL;
        }

        sprintf(full, "%s/%s", dir, command);
        if (stat(full, &st) == 0)
        {
            free(path_copy);
            return full;
        }

        free(full);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
