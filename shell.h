#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

extern char **environ;

/* path */
char *get_path(void);
char *find_path(char *command);

/* exec */
int execute_command(char **args);

#endif

