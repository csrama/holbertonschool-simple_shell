#ifndef SHELL_H
#define SHELL_H

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>  /* نضيفها للـ isspace */

/* External environment */
extern char **environ;

/* Function prototypes */
void display_prompt(void);
void execute_command(char *command);
char *trim_spaces(char *str);  /* دالة جديدة */

#endif /* SHELL_H */
