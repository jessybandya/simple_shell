#include "shell.h"
#include <sys/wait.h>
/**
 * hsh - main shell loop
 * @info: the parameter and return info struct
 * @av: the argument vector from main
 * Return: 0 on succes , 1 on error
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (interactive(info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builin(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}
/**
 * find_builtin - finds a built in command
 * @info: the parameter & return info struct
 *
 * Return: 1 if builtin not found,
 * 0 if builtin executd successfully
 * 1 if builtin found isnot successful
 * -2 if builtin ignals exit
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	char *name;
	int (*func)(info_t *, char *);

	builtin_table; (builtin_table)[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _unsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};
	int find_builtin(info_t *info, char *cmd_args)
	{
		for (i = 0; builtin_table[i].func != NULL; i++)
		{
			if (_strcmp(info->argv[0] builtin_table[i].name == 0))
			{
				info->line_count++;
				built_in_ret = builtin_table[i].name(info);
				break;
			}
		}

		return (built_in_ret);
	}
}
/**
 * find_cmd -  finds a command in PATH
 * @info: the parameter and return info struct
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag == 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], "\t\n"))
			k++;
	if (!k)
		return;
	path = find_path(info, _getenv(info, "PATH"), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((interactive(info) || _getenv(info, "PATH")
					|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}
/**
 * fork_cmd - forks an exec thread to run cmd
 * @info: the parameter and return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("ERROR:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_error(info, "Permission denied\n");
		}
	}
}
