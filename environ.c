#include "shell.h"
/**
 * _myenv - prints the current environment
 * @info: structure containing potential arguments.used to maintain
 * a constant function prototype
 * Return: Always 0
 */
int _myenv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}
/**
 * _getenv - gets the valur of an environ variable
 * @info: structure containing potential arguments
 * @name: env var name
 *
 * Return: Always 0
 */
char *_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node  = node->next;
	}
	return (NULL);
}
/**
 * _mysetenv - intialise a new environment variable or
 * modify a new one
 * @info: Structure containing potential arguments
 * used to maintain constant function prototype
 *
 * Return: Always 0
 */
int _mysetenv(info_t *info)
{
	int i;

	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @info: structure containing potential arguments
 * Return: Always 0
 */
int populate_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	info->env = node;

	return (0);
}
