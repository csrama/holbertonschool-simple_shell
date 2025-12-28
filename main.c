#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *args[64];

    prog_name = argv[0];
    line_number = 0;

    while (1)
    {
        line_number++;
        if (isatty(STDIN_FILENO))
            printf("($) ");

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            exit(0);
        }

        line[read - 1] = '\0'; /* remove newline */

        if (strlen(line) == 0)
            continue;

        /* parse arguments */
        int i = 0;
        char *token = strtok(line, " ");
        while (token)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            exit(0);
        }

        execute_command(args);
    }

    return 0;
}

