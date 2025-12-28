#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

/* Macros */
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
#define MAX_PATH_LEN 4096
#define DELIMITERS " \t\n"

/* Function Prototypes */
void display_prompt(void);
char *read_input(void);
char **parse_input(char *input);
int execute_command(char **args);
char *find_command_in_path(char *command);
int check_command_exists(char *command);
void free_args(char **args);
int is_builtin(char **args);
void handle_builtin(char **args, char *input);

#endif /* MAIN_H */
