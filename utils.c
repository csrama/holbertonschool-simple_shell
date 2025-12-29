#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "shell.h"

/* Trim leading/trailing spaces */
char *trim_spaces(char *str)
{
    char *end;

    if (!str)
        return NULL;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = '\0';
    return str;
}

/* Tokenize a line into args */
char **tokenize(char *line)
{
    char **args = NULL;
    char *token;
    int count = 0;

    line = trim_spaces(line);
    if (!line || *line == '\0')
        return NULL;

    token = strtok(line, " \t\n");
    while (token)
    {
        args = realloc(args, sizeof(char *) * (count + 2));
        args[count] = strdup(token);
        count++;
        token = strtok(NULL, " \t\n");
    }
    if (args)
        args[count] = NULL;

    return args;
}
                                                                                                                                                                                                                                                                                                                                        
