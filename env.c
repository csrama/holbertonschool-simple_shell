#include "shell.h"

/* Return pointer to value after NAME= in envp, or NULL if not found */
char *get_env_value(char **envp, const char *name)
{
    size_t nlen;
    int i;

    if (!envp || !name)
        return NULL;

    nlen = strlen(name);

    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], name, nlen) == 0 && envp[i][nlen] == '=')
            return envp[i] + nlen + 1;
    }
    return NULL;
}

/* strdup replacement using allowed functions */
char *strdup_allowed(const char *s)
{
    char *p;
    size_t n;

    if (!s)
        return NULL;

    n = strlen(s) + 1;
    p = malloc(n);
    if (!p)
        return NULL;

    strcpy(p, s);
    return p;
}

/* Get PWD from envp, else fallback to getcwd into static buffer */
char *get_pwd(char **envp)
{
    char *pwd;
    static char buf[1024];

    pwd = get_env_value(envp, "PWD");
    if (pwd && pwd[0] != '\0')
        return pwd;

    if (getcwd(buf, sizeof(buf)) != NULL)
        return buf;

    return NULL;
}
