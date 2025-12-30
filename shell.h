#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stddef.h>

/* Global environment */
extern char **environ;

/* Global variables for error reporting */
extern char *prog_name;
extern unsigned int line_number;

/* Prototypes */
int execute_command(char **args);
char *find_path(const char *command);
char *get_path(void);
int resolve_command(char *cmd, char *full, size_t size, char **envp);
/* add this prototype */
int check_builtin(char **args);
#endif
