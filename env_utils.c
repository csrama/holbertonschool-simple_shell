#include <string.h>

extern char **environ;

/* Returns the value of the PATH environment variable, or NULL if not found */
char *get_path_value(void)
{
    int i = 0;
    size_t len = 5; /* length of "PATH=" */

    if (!environ)
        return NULL;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", len) == 0)
            return environ[i] + len;  /* return pointer just after "PATH=" */
        i++;
    }
    return NULL;
}

