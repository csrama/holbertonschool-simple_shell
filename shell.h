#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>

extern char **environ;
extern char *prog_name;
extern unsigned int line_number;

char *get_path(void);
char *find_path(char *command);
int execute_command(char **args);

#endif

