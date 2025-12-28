#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>

extern char **environ;

int execute_command(char **args);
char *find_path(char *command);
char *get_path(void);

extern char *prog_name;
extern unsigned int line_number;

#endif

