#include "shell.h"

/**
 * trim_spaces - Removes leading and trailing spaces/tabs in-place
 * @str: String to trim
 * Return: Pointer to trimmed string (inside the same buffer)
 */
char *trim_spaces(char *str)
{
    char *end;

    if (!str)
        return NULL;

    while (*str == ' ' || *str == '\t')
        str++;

    if (*str == '\0')
        return str;

    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        end--;

    *(end + 1) = '\0';
    return str;
}
