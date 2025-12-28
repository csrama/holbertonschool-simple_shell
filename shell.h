#ifndef SHELL_H
#define SHELL_H

/* Add these declarations if not already present */
char *find_command_in_path(char *command);
int check_command_exists(char *command);
int execute_command(char **args);

#endif /* SHELL_H */
