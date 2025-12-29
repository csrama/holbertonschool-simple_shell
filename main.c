#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

char *prog_name = "hsh";
unsigned int line_number = 0;

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    char **args;
    ssize_t nread;

    while (1)
    {
        printf("$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        line_number++;
        args = tokenize(line);
        if (args)
        {
            execute_command(args);

            for (int i = 0; args[i]; i++)
                free(args[i]);
            free(args);
        }
    }

    free(line);
    return 0;
}

