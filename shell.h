#ifndef SHELL_H
#define SHELL_H

char *find_path(char *command);
int execute_command(char **args);
char *get_path_value(void);

extern char *prog_name;
extern unsigned int line_number;
extern char **environ;

#endif

