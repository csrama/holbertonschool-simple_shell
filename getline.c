#include "shell.h"
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

ssize_t _getline(char **lineptr, size_t *n)
{
	static char buffer[BUFFER_SIZE];
	static ssize_t buffer_pos;
	static ssize_t buffer_read;

	ssize_t i = 0;
	char c;
	char *tmp;

	if (!lineptr || !n)
		return (-1);

	if (*lineptr == NULL || *n == 0)
	{
		*n = BUFFER_SIZE;
		*lineptr = malloc(*n);
		if (!*lineptr)
			return (-1);
	}

	while (1)
	{
		if (buffer_pos >= buffer_read)
		{
			buffer_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			buffer_pos = 0;

			if (buffer_read <= 0)
				return (i > 0 ? i : -1);
		}

		c = buffer[buffer_pos++];

		if (i + 1 >= (ssize_t)*n)
		{
			*n *= 2;
			tmp = realloc(*lineptr, *n);
			if (!tmp)
				return (-1);
			*lineptr = tmp;
		}

		(*lineptr)[i++] = c;

		if (c == '\n')
			break;
	}

	(*lineptr)[i] = '\0';
	return (i);
}
