/**
 * trim_spaces - Removes leading and trailing spaces from a string
 * @str: The string to trim
 *
 * Return: Pointer to the trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	/* Trim leading space */
	while (*str == ' ' || *str == '\t' || *str == '\n' ||
	       *str == '\r' || *str == '\f' || *str == '\v')
		str++;

	if (*str == 0)  /* All spaces? */
		return (str);

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' ||
			     *end == '\r' || *end == '\f' || *end == '\v'))
		end--;

	/* Write new null terminator */
	*(end + 1) = '\0';

	return (str);
}
