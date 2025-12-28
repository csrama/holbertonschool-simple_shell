#include "main.h"

/**
 * main - Entry point for the simple shell
 * @argc: Argument count
 * @argv: Argument vector
 * @env: Environment variables
 *
 * Return: 0 on success
 */
int main(int argc, char **argv, char **env)
{
    char *input = NULL;
    char **args = NULL;
    int status = 0;
    (void)argc;
    (void)env;

    while (1)
    {
        /* Display prompt if in interactive mode */
        if (isatty(STDIN_FILENO))
            display_prompt();

        /* Read input */
        input = read_input();
        if (input == NULL)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* Parse input */
        args = parse_input(input);
        if (args == NULL || args[0] == NULL)
        {
            free(input);
            free_args(args);
            continue;
        }

        /* Check for built-in commands */
        if (is_builtin(args))
        {
            handle_builtin(args, input);
            free(input);
            free_args(args);
            continue;
        }

        /* Execute command */
        status = execute_command(args);

        /* Cleanup */
        free(input);
        free_args(args);
    }

    return (0);
}
