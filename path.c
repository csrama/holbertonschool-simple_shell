#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *find_path(char *command)
{
    char *path, *path_copy, *dir, *full_path;
    size_t len;

    if (strchr(command, '/'))
        return (access(command, X_OK) == 0 ? strdup(command) : NULL);

    path = getenv("PATH");
    if (!path || path[0] == '\0')
        return NULL;

    path_copy = strdup(path);
    dir = strtok(path_copy, ":");

    while (dir)
    {
        len = strlen(dir) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
            break;

        sprintf(full_path, "%s/%s", dir, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

