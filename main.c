#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *prog_name;
unsigned int line_number = 0;

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    char *args[2];
    ssize_t read;

    (void)argc;
    prog_name = argv[0];

    while ((read = getline(&line, &len, stdin)) != -1)
    {
        line_number++;

        if (read > 1)
        {
            line[strcspn(line, "\n")] = '\0';
            args[0] = line;
            args[1] = NULL;

            execute_command(args);
        }
    }

    free(line);
    return 0;
}

