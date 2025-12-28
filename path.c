#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

/* get PATH from environment */
char *get_path(void)
{
    int i = 0;

    if (!environ)
        return NULL;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
            return environ[i] + 5;
        i++;
    }
    return NULL;
}

/* find full path of command */
char *find_path(char *command)
{
    char *path = get_path();
    char *path_copy, *dir;
    char *full;
    struct stat st;

    if (!path)
        return NULL;

    if (strchr(command, '/'))
    {
        if (stat(command, &st) == 0)
            return strdup(command);
        else
            return NULL;
    }

    path_copy = strdup(path);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        full = malloc(strlen(dir) + strlen(command) + 2);
        if (!full)
            return NULL;
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

