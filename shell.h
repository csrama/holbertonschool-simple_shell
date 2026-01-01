#ifndef SHELL_H
#define SHELL_H

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

/*  MACROS  */
#define BUFFER_SIZE 1024

/* GLOBALS */
extern char **environ;
extern char *prog_name;
extern unsigned int line_number;

/*  FUNCTION PROTOTYPES */

/* getline replacement */
ssize_t _getline(char **lineptr, size_t *n);

/* command execution */
int execute_command(char **args);

/* helpers (if you already have them / will add later) */
char *_getenv(const char *name);
char *_which(char *command);

#endif /* SHELL_H */
