#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

extern char **environ;
extern char *prog_name;
extern unsigned int line_number;

/* Execute a command */
int execute_command(char **args);

/* Find full path of a command */
char *find_path(const char *command);

#endif
