#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

/**
	* split_line - split input line into tokens
	* @line: input string
	*
	* Return: array of tokens
	*/
char **split_line(char *line)
{
	char **tokens = malloc(sizeof(char *) * 64);
	char *token;
	int i = 0;

	if (!tokens)
	return (NULL);

	token = strtok(line, " \t\n");
	while (token)
	{
	tokens[i++] = token;
	token = strtok(NULL, " \t\n");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
	* free_tokens - free tokens array
	* @tokens: tokens
	* @tokens: array of tokens
	*/
void free_tokens(char **tokens)
{
	free(tokens);
}

/**
	* execute_command - execute a command
	* @ctx: shell context
	* @line: input line
	*/
void execute_command(shell_ctx_t *ctx, char *line)
{
	char **args;
	char *cmd_path;
	pid_t pid;
	int status;

	args = split_line(line);
	if (!args || !args[0])
	{
	free_tokens(args);
	return;
	}

	cmd_path = find_command(args[0], ctx);
	if (!cmd_path)
	{
	fprintf(stderr, "%s: not found\n", args[0]);
	ctx->last_status = 127;
	free_tokens(args);
	return;
	}

	pid = fork();
	if (pid == 0)
	{
	execve(cmd_path, args, ctx->env);
	perror("execve");
	exit(126);
	}
	else
	{
	wait(&status);
	if (WIFEXITED(status))
	ctx->last_status = WEXITSTATUS(status);
	}

	free(cmd_path);
	free_tokens(args);
}
