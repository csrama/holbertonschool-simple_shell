#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>

extern char *prog_name;
extern unsigned int line_number;
extern char **environ;

char *find_path(char *command);
char *get_path_value(void);
int execute_command(char **args);
char *trim_spaces(char *str);
char **tokenize(char *line);

#endif

