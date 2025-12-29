#include <string.h>

extern char **environ;

/* Return the value of PATH, or NULL if not found */
char *get_path_value(void)
{
    int i = 0;
    size_t len = 5; /* length of "PATH=" */

    if (!environ)
        return NULL;

    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", len) == 0)
            return environ[i] + len;  /* return pointer after "PATH=" */
        i++;
    }
    return NULL;
}

